#include <lib.h>
#include <dirs.h>
#include <cfg.h>
#include <commands.h>
#include <save.h>
#include <daemons.h>
#include NETWORK_H

#define WEB_SOURCE_PORT 80

string array allnames = ({});
string array tmpnames;
string reverts_dir, revert_name;
static string upgrade_prefix = "/code/upgrades/"+mudlib_version();
static string reverts_prefix = "/secure/upgrades/reverts/"+mudlib_version();
static string upgrades_txt = "/secure/upgrades/txt";
static string upgrades_files = "/secure/upgrades/files";
static int i = 0;
static int oob = 0;
static object player = 0;
int patched, transver = 0;
static mapping NewFiles = ([]);
static string SaveFile;

void create(){
#ifdef ENABLE_INSTANCES
    SaveFile = save_file(SAVE_LIVEUPGRADE);
    if( unguarded( (: file_exists(SaveFile) :) ) ){
        unguarded( (: restore_object(SaveFile) :) );
    }
#endif

    if(false()){
        object uu = find_object("/secure/daemon/update");
        if(uu) uu->eventDestruct();
        if(uu) destruct(uu);
        patched = 1;
        unguarded( (: cp("/secure/daemon/update.patch",
                        "/secure/daemon/update.c") :) );
        unguarded( (: reload("/secure/daemon/update.c") :) );
    }
}

int eventDestruct(){
#ifdef ENABLE_INSTANCES
    if(directory_exists(path_prefix(SaveFile))){
        unguarded( (: save_object(SaveFile) :) );
    }
#endif
    destruct();
    return 1;
}

int eventBackup(string file){
    string tmp, time_str,short_name,filename;
    if(!file) return 0;
    if(!file_exists(file)) return -1;
    if( !(tmp = read_file(file)) ) return -2;  
    time_str = time()+"."+random_numbers(5);
    short_name = last_string_element(file,"/");
    if(!revert_name){
        revert_name = itoa(time());
    }
    if(!reverts_prefix){
        reverts_prefix = "/secure/upgrades/reverts/"+mudlib_version();
    }
    if(!directory_exists("/secure/upgrades/reverts/")){
        mkdir("/secure/upgrades/reverts");
    }
    if(!directory_exists(reverts_prefix)){
        mkdir(reverts_prefix);
    }
    if(!directory_exists(reverts_prefix+"/"+revert_name)){
        mkdir(reverts_prefix+"/"+revert_name);
    }
    filename = reverts_prefix+"/"+revert_name+
        "/"+short_name+"."+time_str;
    write_file(reverts_prefix+"/"+revert_name+"/"+
            "/bk.db",short_name+"."+time_str+" : "+file+"\n");
    cp(file,filename);
    return 1;
}

int eventRevert(string revert_path){
    string *files;

    if(!revert_path){
        return 0;
    }

    if(!directory_exists(revert_path)){
        return -1;
    }

    if(!file_exists(revert_path+"/bk.db")){
        return -2;
    }

    files = explode(read_file(revert_path+"/bk.db"),"\n");

    if(!files || !sizeof(files)){
        return -3;
    }

    foreach(string line in files){
        string backup, target;
        if(sscanf(line,"%s : %s", backup, target) != 2) continue;
        if(!directory_exists(path_prefix(target))){
            mkdir_recurse(path_prefix(target));
        }
        cp(revert_path+"/"+backup, target);
    }
    return 1;
}

void eventReloads(){
    cp("/secure/daemon/update.patch","/secure/daemon/update.c");
    reload(UPDATE_D);
    reload(LUGET_D);
}

int eventCopy(string element){
    string path;
    if(!NewFiles[element]) return 0;
    path = path_prefix(NewFiles[element]);
    if(!directory_exists(path)) mkdir_recurse(path);
    rename(element, NewFiles[element]);
    return 1;
}

mixed cmd(string str) {
    string mud = "Dead Souls"; 
    string file;
    string orig_str = str;
    int foo, tmpint = 0;
    object inet = find_object(INET_D);
    string *preload_file = explode(read_file(CFG_PRELOAD),"\n");
    mixed *socks = socket_status();

    NewFiles = ([]);

    if(!revert_name){
        revert_name = itoa(time());
    }

    if(!str || str == ""){
        write("Try: help liveupgrade.");
        return 1;
    }

    if(str == "alpha"){
        if(transver){
            transver = 0;
            write("Alpha/Stable upgrades disabled.");
            return 1;
        }
        if(!transver){
            transver = 1;
            write("Alpha/Stable upgrades enabled.");
            return 1;
        }
    }

    if(str == "revert"){
        string *vers = get_dir("/secure/upgrades/reverts/");
        string ver, subver;
        if(!vers || !sizeof(vers)){
            write("There is no previous backup to revert to.");
            return 1;
        }
        else ver = vers[0];
        vers = get_dir("/secure/upgrades/reverts/"+ver+"/");
        if(!vers || !sizeof(vers)){
            write("There is no backup instance to revert to.");
            return 1;
        }
        else subver = "/secure/upgrades/reverts/"+ver+"/"+vers[0];
        eventRevert(subver);
        rename(subver,"/secure/upgrades/bak/"+last_string_element(subver,"/"));
        rmdir(path_prefix(subver));
        write("Reversion complete.");
        return 1;
    } 

    foreach(mixed element in socks){
        if(element[1] == "DATA_XFER" && element[4] == WEB_SOURCE_IP+"."+WEB_SOURCE_PORT &&
                str != "cancel"){
            player->eventPrint("A download is still in progress. Please wait until it is complete.");
            return 1;
        }
    }

    if(!player && this_player()) player = this_player();
    allnames = ({});
    if(!player) return 0;

    if( !(master()->valid_apply(({ "SECURE" }))) )
        error("Illegal attempt to access liveupgrade: "+get_stack()+" "+identify(previous_object(-1)));

    if(!directory_exists("/secure/upgrades/bak")){
        mkdir("/secure/upgrades/bak");
    }

    if(!directory_exists(reverts_prefix)){
        mkdir(reverts_prefix);
    }

    i = sscanf(mudlib_version(),"2.3a%d",tmpint);
    if(i && tmpint < 12) oob = 1;
    else {
        if(!strsrch(str,"-o ")){
            oob = 1;
            str = replace_string(str,"-o ","");
        }
        else oob = 0;
    }

    i = 0;
    foo = sscanf(str,"%s %s",file, mud);
    if(!foo || foo < 2) file = str;
    if(str == "apply" || str == "deferred"){
        string *files = ({});
        string nlu, secs = upgrades_files+"/0^0secure0^0include0^0secrets.h";
        object nlob;
        if(file_exists(SECRETS_H)){
            catch( cp(SECRETS_H, "/secure/save/backup/secrets_" + 
                        imc2_mud_name() + "." + time() + ".bak") );
        }
        if(file_exists(secs)) catch( rm(secs) );
        nlu = upgrades_files+"/0^0secure0^0cmds0^0admins0^0liveupgrade.c";
        if(file_exists(nlu)){
            catch( nlob = load_object(nlu) );
            if(nlob && nlob->GetDeferment() && str != "deferred"){
                nlob->eventDestruct();
                return 1;
            }
            if(nlob) nlob->eventDestruct();
            if(!rename(nlu, CMD_LIVEUPGRADE)){
                write("The liveupgrade command has been updated.");
                write("Please wait five seconds, then again type:\n");
                write("liveupgrade apply");
                if(!(this_object()->eventDestruct())){
                    RELOAD_D->eventReload(this_object(), 0);
                }
                return 1;
            }
        }
        player->eventPrint("I hope you backed up...\n");
        foreach(string element in get_dir(upgrades_files+"/")){
            if(element == "0^0secure0^0sefun0^0mud_info.c"){
                object thingy = load_object(upgrades_files+"/"+element);
                string vers;
                if(thingy){
                    string current_ver = mudlib_version();
                    vers = thingy->mudlib_version();
                    if(((grepp(vers,"a") && !grepp(current_ver, "a")) ||
                                (!grepp(vers,"a") && grepp(current_ver, "a"))) &&
                            !transver){
                        write("This upgrade would cross stable/alpha "
                                "boundaries, but that has not been enabled "
                                "with \"liveupgrade alpha\" yet.");
                        return 1;
                    }
                }
            }
            files += ({ upgrades_files+"/"+element });
        }
        foreach(string element in files){
            string contents = "";
            NewFiles[element] = replace_string(replace_string(element,"0^0","/"),
                    upgrades_files+"/","");
            contents = read_file(element);
            if(!contents) contents = "";
            if(last(contents,1) != "\n") contents += "\n";
            write_file(element, contents, 1);
            reset_eval_cost();
            call_out( (: eventBackup :), 0, NewFiles[element]);
            if(directory_exists(NewFiles[element])) true();
            else {
                call_out( (: eventCopy :), 0, element);
            }
        }
        if(member_array(INET_D,preload_file) == -1 && inet) inet->eventDestruct();
        call_out( (: eventReloads :), 10);
        patched = 0;
        RELOAD_D->eventReload(this_object(), 15);
        rm("/secure/upgrades/txt/list.txt");
        player->eventPrint("\nAlmost done...");
        player = 0;
        return 1;
    }
    if(str == "cancel"){
        string *files = ({});
        foreach(string element in get_dir(upgrades_files+"/")){
            files += ({ upgrades_files+"/"+element });
        }
        foreach(string element in files){
            rm(element);
        }
        rm("/secure/upgrades/txt/list.txt");
        player->eventPrint("Cancelled.");
        player = 0;
        RELOAD_D->eventReload(this_object(), 2);
        reload(LUGET_D);
        return 1;
    }
    if(oob){
        if(!inet){
            inet = load_object(INET_D);
            player->eventPrint("Starting INET_D.");
            if(member_array(INET_D,preload_file) == -1)
                player->eventPrint("When you complete the upgrade by using the \"apply\" keyword, the "
                        "inet daemon will be shut down, since you do not have it enabled by "
                        "default. Please remember to either apply the upgrades when the downloading "
                        "is complete, or manually shut down INET_D with the command: mudconfig inet stop\n");
        }
        if(!inet){
            player->eventPrint("There is a problem with INET_D. The upgrade will not proceed.");
            return 1;
        }

        if(!INET_D->GetService("oob")){
            player->eventPrint("The OOB service is not enabled. Enabling it now.");
            INET_D->AddService("oob",OFFSET_OOB,LIB_OOB,0);
        }

        if(!INET_D->GetService("oob")){
            player->eventPrint("There was a problem enabling the OOB service. The upgrade will not proceed.");
            return 1;
        }

        if(!INET_D->GetServer("oob")){
            player->eventPrint("The OOB service is not started. Starting it now.");
            INET_D->eventStartServer("oob");
        }

        if(!INET_D->GetServer("oob")){
            player->eventPrint("There was a problem starting the OOB service. The upgrade will not proceed.");
            return 1;
        }

        if(foo < 2) {
            mud = LIVEUPGRADE_SERVER;
            file = str;
        }
        if(!file){
            return this_object()->GetHelp();
        }

        mud = INTERMUD_D->GetMudName(mud);
        if(!mud){
            player->eventPrint("That liveupgrade server appears unavailable.");
            return 1;
        }
    }
    if(file == "all"){
        string tmp = replace_string(upgrades_txt+"/upgrades.txt","/","0^0");
        if(player && this_player() && player != this_player()){
            this_player()->eventPrint("This command is currently locked and in use by "+capitalize(player->GetKeyName())+".");
            return 1;
        }
        else if(this_player()) player = this_player();
        else player = this_object();

        if(LUGET_D->GetUpgrading()){
            player->eventPrint("An upgrade in already occurring. Please wait for it to complete.");
            return 1;
        }

        if(!file_exists(upgrades_txt+"/list.txt")){
            player->eventPrint("Downloading updates table. Please wait...");
            rename("/secure/upgrades/files","/secure/upgrades/bak/"+time());
            mkdir("/secure/upgrades/files");
            if(oob){
                OOB_D->GetFile(mud,upgrades_txt+"/upgrades.txt");
            }
            else {
                LUGET_D->GetFile(WEB_SOURCE_IP, upgrade_prefix+"/upgrades.txt",WEB_SOURCE_NAME,
                        "/secure/upgrades/txt/list.txt",WEB_SOURCE_PORT);
            }
            call_out( (: cmd :), 5, orig_str);
            return 1;
        }

        tmpnames = explode(read_file(upgrades_txt+"/list.txt"),"\n");
        foreach(string element in tmpnames){
            if(!oob) allnames += ({ "/code/upgrades/"+mudlib_version()+element });
            else allnames += ({ element });
        }

        if(oob){
            OOB_D->eventMajorUpgrade(mud, allnames);
        }
        else {
            LUGET_D->eventMajorUpgrade(WEB_SOURCE_IP, allnames,WEB_SOURCE_NAME);
        }
        rm(upgrades_txt+"/list.txt");
        player->eventPrint("Full upgrade begun.");
        player->eventPrint("Please wait until you receive a completion message,  "+
                "then issue the command: liveupgrade apply\n\n");
        player->eventPrint("%^FLASH%^RED%^WARNING! %^BLACK%^WARNING! %^YELLOW%^WARNING! %^RESET%^WARNING!");
        player->eventPrint("You must *always* do a full backup before applying the liveupgrade. "+
                "If the liveupgrade screwed up, and you get garbage files because of connection "+
                "problems, it may be necessary for you to restore from backup to be able to "+
                "start the mud again. You've been warned.");
        return 1;
    }
    if(oob){
        OOB_D->GetFile(mud,file);
        player->eventPrint("Requesting the file \""+file+"\" from "+INTERMUD_D->GetMudName(mud)+".");
    }
    else {
        player->eventPrint("Requesting the file \""+file+"\" from "+WEB_SOURCE_IP);
        LUGET_D->GetFile(WEB_SOURCE_IP, upgrade_prefix+file);
    }
    return 1;
}

void eventReceiveReport(string str){
    if(player) player->eventPrint(str);
}

int GetDeferment(){
    string *defers = ({ "secrets.h", "network.h", "mssp.h" });
    string secs = upgrades_files+"/0^0secure0^0include0^0";
    foreach(string deferment in defers){
        if(file_exists(secs+deferment) && 
                file_exists("/secure/include/"+deferment)){
            rm(secs+deferment);
        }
    }
    return 0;
}

string GetHelp() {
    return ("Syntax: liveupgrade all\n"
            "        liveupgrade apply\n"
            "        liveupgrade cancel\n"
            "        liveupgrade revert\n"
            "        liveupgrade alpha\n\n"
            "To upgrade all files to the next appropriate level for your lib version:\n"
            "liveupgrade all\n"
            "Wait until you receive the completion message before finalizing the upgrade. "
            "You can finalize the upgrade by typing:\n"
            "liveupgrade apply\n"
            "This will delete your old copies of files and copy the newly downloaded "
            "ones in their place.\n"
            "NEVER EVER do a liveupgrade without a full backup first.\n"
            "To cancel the liveupgrade process:\n"
            "liveupgrade cancel\n"
            "To restore your mud to the condition it was in prior to the last liveupgrade.\n"
            "liveupgrade revert\n"
            "To enable liveupgrading between alpha and stable versions:\n"
            "liveupgrade alpha\n\n"
            "Web proxies are *NOT* supported. OOB is no longer supported.");
}
