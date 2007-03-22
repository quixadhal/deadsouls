#include <lib.h>
#include <dirs.h>
#include <cfg.h>
#include <daemons.h>
#include <network.h>

#define WGET_D "/secure/daemon/wget"
#define WEB_SOURCE "149.152.218.102"
//#define WEB_SOURCE "127.0.0.1"
#define WEB_SOURCE_NAME "dead-souls.net"
#define WEB_SOURCE_PORT 80

inherit LIB_DAEMON;

string array allnames = ({});
string array tmpnames;
string upgrade_prefix = "/code/upgrades/"+mudlib_version();
string upgrades_txt = "/secure/upgrades/txt";
string upgrades_files = "/secure/upgrades/files";
int i = 0;
int oob = 0;
object player = 0;

mixed cmd(string str) {
    string mud = "Dead Souls"; 
    string file;
    string orig_str = str;
    int foo, tmpint = 0;
    mapping NewFiles = ([]);
    object inet = find_object(INET_D);
    string *preload_file = explode(read_file(CFG_PRELOAD),"\n");
    mixed *socks = socket_status();

    foreach(mixed element in socks){
	if(element[1] == "DATA_XFER" && element[4] == WEB_SOURCE+"."+WEB_SOURCE_PORT &&
	  str != "cancel"){
	    player->eventPrint("A download is still in progress. Please wait until it is complete.");
	    return 1;
	}
    }

    if(!player && this_player()) player = this_player();
    allnames = ({});
    if(!player) return 0;

    if( !((int)master()->valid_apply(({ "SECURE" }))) )
	error("Illegal attempt to access liveupgrade: "+get_stack()+" "+identify(previous_object(-1)));

    if(!directory_exists("/secure/upgrades/bak")){
	mkdir("/secure/upgrades/bak");
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
    if(str == "apply"){
	string *files = ({});
	player->eventPrint("I hope you backed up...\n");
	foreach(string element in get_dir(upgrades_files+"/")){
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
	    load_object("/secure/cmds/creators/bk")->cmd(NewFiles[element]);
	    if(directory_exists(NewFiles[element])) rm(element);
	    else {
		string path = path_prefix(NewFiles[element]);
		if(!directory_exists(path)) mkdir(path);
		rename(element, NewFiles[element]);
	    }
	}
	if(member_array(INET_D,preload_file) == -1 && inet) inet->eventDestruct();
	reload(UPDATE_D);
	RELOAD_D->eventReload(this_object(), 2);
	reload(WGET_D);
	rm("/secure/upgrades/txt/list.txt");
	player->eventPrint("\nDone. It is now a very good time to reboot the mud.");
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
	reload(WGET_D);
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

	if(WGET_D->GetUpgrading()){
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
		WGET_D->GetFile(WEB_SOURCE, upgrade_prefix+"/upgrades.txt",WEB_SOURCE_NAME,
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
	    WGET_D->eventMajorUpgrade(WEB_SOURCE, allnames,WEB_SOURCE_NAME);
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
	player->eventPrint("Requesting the file \""+file+"\" from "+WEB_SOURCE);
	WGET_D->GetFile(WEB_SOURCE, upgrade_prefix+file);
    }
    return 1;
}

void eventReceiveReport(string str){
    if(player) player->eventPrint(str);
}

string GetHelp() {
    return ("Syntax: liveupgrade [-o] all\n"
      "        liveupgrade apply\n"
      "        liveupgrade cancel\n"
      "To use oob updates (not recommended), use the -o flag. The default "
      "is currently an http connection to dead-souls.net, which is vastly "
      "faster and more secure than oob.\n"
      "To upgrade all files to the next appropriate level for your lib version:\n"
      "liveupgrade all\n"
      "Wait until you receive the completion message before finalizing the upgrade. "
      "You can finalize the upgrade by typing:\n"
      "liveupgrade apply\n"
      "This will delete your old copies of files and copy the newly downloaded "
      "ones in their place.\n"
      "NEVER EVER do a liveupgrade without a full backup first."
      "");
}
