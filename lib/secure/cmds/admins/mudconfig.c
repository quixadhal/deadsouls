#include <lib.h>
#include <cfg.h>
#include <save.h>
#include <daemons.h>
#include <commands.h>
#include NETWORK_H
#include <sockets.h>
string GetHelp();

inherit LIB_DAEMON;

string ret_string = "";
string array config, config2, keywords;
string array bools = ({ "enable","disable","on","off","1","0" });
string array yesbools = ({ "enable","on","1","yes" });
string array nobools = ({ "disable","off","0","no" });
string array restrict_tokens = ({ "restrict","unrestrict" });
string array nonmodals = ({ "liveupgrade", "prompt","status","email",
        "websourceip", "websourcename", "mudname", "mudport",
        "debugger", "access", "pinging", "pinginterval",
        "imc2serverpass", "imc2clientpass" });
string array antimodals = ({ "imc2", "ced" });
string array modals = antimodals + ({ "channelpipes", "fastcombat", 
        "catchtell","matchcommand", "matchobject", "autowiz", "locked",
        "localtime", "justenglish", "justhumans", "encumbrance", "pk", 
        "compat", "exitsbare", "nmexits", "grid", "minimap", "wizmap",
        "cgi", "dirlist", "creweb", "selectclass", "severable",
        "retain", "defaultparse", "disablereboot", "loglocal", "logremote",
        "questrequired", "autoadvance","guestallowed", "playerintertell" });
string array inet_services = ({ "oob", "hftp", "ftp", "http", "rcp", "inet" });

static int NotImplemented(string which);
varargs static int TestFun(string which, string arg);
varargs static int ModIntermud(string which, string arg);
varargs static int ModRouter(string which, string arg);
static int ProcessModal(string which, string arg);
varargs static int ModStartRoom(string which, string arg);
static int ProcessOther(string which, string arg);
static int ProcessString(string which, string arg);
int ProcessInet(string which, string arg);
varargs static int ModDefaultDomain(string which, string arg);
varargs static int ModCfg(string which, string arg);

static private void validate() {
    if(!this_player()) return 0;
    if( !(master()->valid_apply(({ "ASSIST" }))) )
        error("Illegal attempt to access mudconfig: "+get_stack()+" "+identify(previous_object(-1)));
}

mixed cmd(string str) {
    string which, arg;

    validate();

    if(this_player() && !securep(this_player())){
        write("This command is for full admins only. To become a "+
                "full admin, you'll need to be added to the SECURE group.");
        return 1;
    }

    if(!this_player() && base_name(previous_object()) != IMC2_D &&
            base_name(previous_object()) != INTERMUD_D) return 1;

    if(base_name(previous_object()) == IMC2_D){
        if(str != "imc2 disable") return 1;
    }
    if(base_name(previous_object()) == INTERMUD_D){
        if(str != "intermud disable") return 1;
    }

    if(!str || sscanf(str, "%s %s", which, arg) != 2){
        write(GetHelp());
        return 1;
    }
    cp(CONFIG_H,"/secure/save/backup/config."+time());
    config = explode( read_file(CONFIG_H),"\n" );
    config2 = ({});
    keywords = ({});
    foreach(string element in config){
        string s1, s2;
        if(sscanf(element,"#define %s %s",s1,s2) == 2) keywords += ({ s1 });
    }

    if(member_array(which,modals) != -1){
        ProcessModal(which, arg);
        return 1;
    }

    if(member_array(which,inet_services) != -1){
        ProcessInet(which, arg);
        return 1;
    }

    else switch(which){
        case "test" : TestFun(which, arg);break;
        case "intermud" : ModIntermud(which, arg);break;
        case "router" : ModRouter(which, arg);break;
        case "startroom" : ModStartRoom(which, arg);break;
        case "defaultdomain" : ModDefaultDomain(which, arg);break;
        case "resets" : which = "TIME_TO_RESET";ProcessOther(which,arg);break;
        case "offset" : which = "GMT_OFFSET";ProcessOther(which,arg);break;
        case "extraoffset" : which = "EXTRA_TIME_OFFSET";ProcessOther(which,arg);break;
        case "newbielevel" : which = "MAX_NEWBIE_LEVEL";ProcessOther(which,arg);break;
        case "monitor" : which = "GLOBAL_MONITOR";ProcessOther(which,arg);break;
        case "maxip" : which = "SAME_IP_MAX";ProcessOther(which,arg);break;
        case "pinginterval" : which = "PING_INTERVAL";ProcessOther(which,arg);break;
        case "maxcommands" : which = "MAX_COMMANDS_PER_SECOND";ProcessOther(which,arg);break;
        case "maxidle" : which = "IDLE_TIMEOUT";ProcessOther(which,arg);break;
        case "instances" : which = "ENABLE_INSTANCES";ProcessOther(which,arg);break;
        case "hostip" : which = "HOST_IP";ProcessString(which,arg);break;
        case "email" : which = "ADMIN_EMAIL";ProcessString(which,arg);break;
        case "liveupgrade" : which = "LIVEUPGRADE_SERVER";ProcessString(which,arg);break;
        case "mudstatus" : which = "MUD_STATUS";ProcessString(which,arg);break;
        case "debugger" : which = "DEBUGGER";ProcessString(which,arg);break;
        case "websourceip" : which = "WEB_SOURCE_IP";ProcessString(which,arg);break;
        case "websourcename" : which = "WEB_SOURCE_NAME";ProcessString(which,arg);break;
        case "imc2clientpass" : which = "IMC2_CLIENT_PW";ProcessString(which,arg);break;
        case "imc2serverpass" : which = "IMC2_SERVER_PW";ProcessString(which,arg);break;
        case "mudname" : ModCfg("name", arg);break;
        case "mudport" : ModCfg("port", arg);break;
        default : NotImplemented(which);break;
    }
    return 1;
}

varargs static int CompleteConfig(string file){
    string ret = implode(config2,"\n")+"\n";
    if(!file) file = CONFIG_H;
    validate();
    ret = replace_string(ret,"\n\n","\n");
    write_file(file,ret,1);
    RELOAD_D->ReloadBaseSystem();
    reload(LIB_CONNECT,0,1);
    reload(BANISH_D,0,1);
    reload(PLAYERS_D,0,1);
    write("Command complete.");
    return 1;
}

int ModPortOffset(string which, string arg){
    string out, service, svc, junk, offset, new_offset;
    string *new_array = ({});
    string netcfg = read_file(NETWORK_H);
    string *net_array = explode(netcfg,"\n");

    new_offset = arg;
    service = which;
    service = upper_case(service);
    foreach(string element in net_array){
        if(sscanf(element,"#define OFFSET_%s %s %d",svc, junk, offset) == 3){
            if(lower_case(svc) == lower_case(service)){
                out = "#define OFFSET_"+service+"               "+new_offset;
            }
            else out = element;
        }
        else out = element;
        new_array += ({ out });
    }
    out = implode(new_array,"\n");

    write_file(NETWORK_H,out,1);
    write("The "+service+" port offset is being set to "+offset+".");
    RELOAD_D->eventReload(this_object(), 1, 1);
    reload(MASTER_D,0,1);
    return 1;
}             

int ModPort(string which, mixed arg){
    string out, service, svc, junk, new_offset, new_port;
    string *new_array = ({});
    string netcfg = read_file(NETWORK_H);
    string *net_array = explode(netcfg,"\n");
    int offset;

    if(stringp(arg)) new_port = arg;
    else new_port = itoa(arg);
    service = which;
    service = upper_case(service);
    foreach(string element in net_array){
        if(sscanf(element,"#define OFFSET_%s %s %d",svc, junk, offset) == 3){
            if(lower_case(svc) == lower_case(service)){
                new_offset = ""+(atoi(new_port) - query_host_port());
                out = "#define OFFSET_"+service+"               "+new_offset;
            }
            else out = element;
        }
        else out = element;
        new_array += ({ out });
    }
    out = implode(new_array,"\n");
    if(last(out,1) != "\n") out += "\n";
    write_file(NETWORK_H,out,1);
    write("The "+service+" port is being set to "+atoi(new_port)+".");
    write("To complete this configuration, wait 2 seconds, then issue the following commands:");
    write("mudconfig "+lower_case(service)+" disable");
    write("mudconfig "+lower_case(service)+" enable");
    RELOAD_D->eventReload(this_object(), 1, 1);
    reload(MASTER_D,0,1);
    return 1;
}

static int NotImplemented(string which){
    validate();
    write("The keyword \""+which+"\" is not yet implemented in mudconfig.");
    return 1;
}

varargs static int TestFun(string which, string arg){
    validate();
    write("Which: "+which+" arg: "+arg);
    return 1;
}

varargs static int ModStartRoom(string which, string arg){
    if(!arg){
        write("Please specify the absolute path to the room's file.");
        return 1;
    }
    if(first(arg,1) != "/"){
        write("You've entered a relative path. Please try again, using an absolute path.");
        return 1;
    }
    if(last(arg,2) == ".c") arg = truncate(arg,2);
    if(!file_exists(arg+".c")){
        write("That file does not exist.");
        return 1;
    }
    if(catch(load_object(arg))){
        write("/n/nThat room file is broken. Please fix it and try again.");
        return 1;
    }
    cp(ROOMS_H,"/secure/save/backup/rooms."+time());
    config = explode( read_file(ROOMS_H),"\n" );
    config2 = ({});
    foreach(string line in config){
        string s1,s2,s3;
        if(sscanf(line,"%s %s %s",s1,s2,s3) == 3){
            if(s1 == "#define" && s2 == "ROOM_START") 
                line = "#define ROOM_START       \""+arg+"\"";
        }
        config2 += ({ line });
    }
    CompleteConfig(ROOMS_H);
    reload(LIB_CREATOR,1,1);
    write("\nNote: Some objects, like verbs and workrooms, still contain the old "+
            "START_ROOM value. This will not change until they are reloaded or "+
            "the mud reboots.");
    return 1;
}

varargs static int ModDefaultDomain(string which, string arg){
    if(!arg){
        write("Please specify the absolute path to the domain, eg: /domains/MystyShyre");
        return 1;
    }

    if(first(arg,1) != "/"){
        write("You've entered a relative path. Please try again, using an absolute path. "+
                "For example: mudconfig defaultdomain /domains/MystyShyre");
        return 1;
    }

    if(!directory_exists(arg)){
        write("That domain does not exist. Type: help domaincreate");
        return 1;
    }

    if(!directory_exists(arg+"/room")){
        write("That directory seems to lack a /room dir. It is not suitable for a domain.");
        return 1;
    }

    cp("/secure/include/dirs.h","/secure/save/backup/dirs."+time());
    config = explode( read_file("/secure/include/dirs.h"),"\n" );
    config2 = ({});
    foreach(string line in config){
        string s1,s2,s3;
        if(sscanf(line,"%s %s %s",s1,s2,s3) == 3){
            if(s1 == "#define" && s2 == "DIR_STANDARD_DOMAIN")
                line = "#define DIR_STANDARD_DOMAIN       \""+arg+"\"";
        }
        config2 += ({ line });
    }

    CompleteConfig("/secure/include/dirs.h");

    write("\nPlease reboot the mud for this change to take effect.\n");
    return 1;
}

varargs static int ModRouter(string which, string arg){
    string preloads = read_file(CFG_PRELOAD);
    string *load_lines = explode(preloads,"\n");
    string *ret_arr = ({});
    string yesline = "/secure/daemon/i3router/server";
    string noline = "#/secure/daemon/i3router/server";
    validate();
    if(member_array(arg,bools) == -1){
        write("Please try again, indicating whether you want to enable or disable the intermud router. Note that this setting will persist across reboots.");
        return 1;
    }

    if(member_array(arg,yesbools) != -1){
        if(member_array(yesline,load_lines) != -1) write("Persistent router activation is already enabled.");
        else {
            write("Enabling persistent router activation.");
            foreach(string line in load_lines){
                if(line == noline) line = yesline;
                ret_arr += ({ line });
            }
            if(member_array(yesline,ret_arr) == -1){
                ret_arr += ({ yesline });
            }
            unguarded( (: cp(CFG_PRELOAD,"/secure/save/backup/preload."+time()) :) );
            ret_string = implode(ret_arr,"\n")+"\n";
            ret_string = replace_string(ret_string,"\n\n","\n");
            unguarded( (: write_file(CFG_PRELOAD,ret_string,1) :) );
        }
        if(find_object(ROUTER_D)) write("The intermud router is already running.");
        else {
            write("Starting the intermud router.");
            load_object(ROUTER_D);
        }

        ret_string = "";
        write("The intermud router is enabled.");
        return 1;
    }

    if(member_array(arg,nobools) != -1){

        if(member_array(noline,load_lines) != -1) write("Persistent router activation is already disabled.");
        else {
            write("Disabling persistent router activation.");
            foreach(string line in load_lines){

                if(line == yesline) line = noline;
                ret_arr += ({ line });
            }
            if(member_array(noline,ret_arr) == -1){
                ret_arr += ({ noline });
            }
            unguarded( (: cp(CFG_PRELOAD,"/secure/save/backup/preload."+time()) :) );
            ret_string = implode(ret_arr,"\n")+"\n";
            ret_string = replace_string(ret_string,"\n\n","\n");
            unguarded( (: write_file(CFG_PRELOAD,ret_string,1) :) );

        }
        if(!find_object(ROUTER_D)) write("The intermud router is not running.");
        else {
            write("Stopping the intermud router.");
            find_object(ROUTER_D)->eventDestruct();
        }
        ret_string = "";
        write("The intermud router is disabled.");
        return 1;
    }
}

varargs static int ModIntermud(string which, string arg){
    validate();
    write("Which: "+which+" arg: "+arg);
    if(arg == "restrict"){
        write("Restricting intermud...");
        flush_messages(this_player());
        foreach(string element in config){
            if(grepp(element, "RESTRICTED_INTERMUD")) element = "#define RESTRICTED_INTERMUD      1";
            config2 += ({ element });
        }
        CompleteConfig();
        reload(INTERMUD_D,0,1);
        reload(IMC2_D,0,1);
        reload("/daemon/services/tell",0,1);
        reload("/daemon/services",0,1);
        return 1;
    }

    if(arg == "reset"){
        object ob = find_object(INTERMUD_D);
        string savei3 = save_file(SAVE_INTERMUD);
        write("Purging all intermud data (including router password!). Previous data file saved to /secure/save/backup/ .");
        if(!ob){
            if(file_exists(savei3)){
                rename(savei3, "/secure/save/backup/intermud."+time());
            }
        }
        else {
            if(file_exists(savei3)){
                rename(savei3, "/secure/save/backup/intermud."+time());
            }
            reload(INTERMUD_D,0,1);
        }
        config2 = config;
        CompleteConfig();
        return 1;
    }

    if(arg == "unrestrict"){
        write("Unrestricting intermud...");
        flush_messages(this_player());
        foreach(string element in config){
            if(grepp(element, "RESTRICTED_INTERMUD")) element = "#define RESTRICTED_INTERMUD      0";
            config2 += ({ element });
        }
        CompleteConfig();
        reload(INTERMUD_D,0,1);
        reload(IMC2_D,0,1);
        reload("/daemon/services/tell",0,1);
        return 1;
    }

    if(member_array(arg,bools) == -1){
        write("Please try again, indicating whether you want to enable, disable, restrict, unrestrict, or reset intermud.");
        return 1;
    }

    if(member_array(arg,yesbools) != -1){
        write("Enabling intermud...");
        flush_messages(this_player());
        foreach(string element in config){
            if(grepp(element, "DISABLE_INTERMUD")) element = "#define DISABLE_INTERMUD         0";
            config2 += ({ element });
        }
        CompleteConfig();
        reload(INTERMUD_D,0,1);
        return 1;
    }

    if(member_array(arg,nobools) != -1){
        write("Disabling intermud...");
        if(this_player()) flush_messages(this_player());
        foreach(string element in config){
            if(grepp(element, "DISABLE_INTERMUD")) element = "#define DISABLE_INTERMUD         1";
            config2 += ({ element });
        }
        CompleteConfig();
        reload(INTERMUD_D,0,1);
        return 1;
    }
    return 1;
}

static int ProcessOther(string which, string arg){
    int junk;
    validate();

    if(sscanf(arg,"%d",junk) != 1){
        write("This parameter requires an integer as an argument.");
        return 1;
    }

    foreach(string element in config){
        if(grepp(element, which)){
            string s1, s2, s3;
            if(sscanf(element,"#define %s %s",s1,s2) != 2){
                write("Major problem. You should revert to a backup of "+
                        CONFIG_H+" immediately.");
                return 1;
            }
            s3 = trim(s2);
            s2 = replace_string(s2,s3,arg);
            element = "#define "+s1+" "+s2;
        }
        config2 += ({ element });
    }
    CompleteConfig();
    if(which == "LOCAL_TIME" || which == "EXTRA_TIME_OFFSET" || which == "GMT_OFFSET"){
        reload("/secure/sefun/local_time",0,1);
        reload("/secure/sefun/time",0,1);
        reload("/secure/sefun/timestamp",0,1);
        reload("/secure/sefun/sefun",0,1);
        RELOAD_D->eventReload("/daemon/time",2);
        reload("/secure/cmds/creators/people",0,1);
        reload("/cmds/players/date",0,1);
        reload("/cmds/players/nextreboot",0,1);
        reload("/cmds/players/version",0,1);
        write("This configuration change will require a few minutes to take effect completely.");
    }
    if(which == "GLOBAL_MONITOR") reload(SNOOP_D,0,1);
    if(which == "IDLE_TIMEOUT" || which == "MAX_NEWBIE_LEVEL" || 
            which == "FAST_COMBAT" || which == "GRID" || which == "WIZMAP" ||
            which == "MINIMAP"){ 
        reload(LIB_CREATOR,1,1);
        write("This configuration will take effect for each user the next time they log in.");
        return 1;
    }
    if(which == "PING_INTERVAL") reload (PING_D,1,1);
    return 1;
}

static int ProcessString(string which, string arg){
    object ob;
    validate();

    if(!strsrch(which, "IMC2_")){
        SECRETS_D->SetSecret(which, arg);
        reload(SECRETS_D, 0, 1);
        reload(IMC2_D, 0, 1);
        config2 = config;
        CompleteConfig();
        return 1;
    }

    foreach(string element in config){
        if(grepp(element, which)){
            string s1, s2, s3;
            if(sscanf(element,"#define %s %s",s1,s2) != 2){
                write("Major problem. You should revert to a backup of "+
                        CONFIG_H+" immediately.");
                return 1;
            }
            s3 = trim(s2);
            s2 = replace_string(s2,s3,"\""+arg+"\"");
            element = "#define "+s1+" "+s2;
        }
        config2 += ({ element });
    }
    if(!strsrch(which, "WEB_SOURCE") 
            && ob = find_object(CMD_LIVEUPGRADE)){
        ob->eventDestruct();
    }
    CompleteConfig();
    return 1;
}

static int ProcessModal(string which, string arg){
    int junk;
    validate();
    if(!arg){
        arg = "no";
        write("No argument: Assuming you want to disable this feature.");
    }
    if(member_array(arg,yesbools) != -1) junk = 1;
    if(member_array(arg,nobools) != -1) junk = 0;
    if(member_array(which,antimodals) != -1) junk = ( junk ^ 1 ); 

    if(junk) junk = 1;
    else junk = 0;
    arg = itoa(junk);

    if(sscanf(arg,"%d",junk) != 1){
        write("This parameter is a modal or quasi-modal. I have a hunch it requires an integer as an argument, or at least a \"yes\" or \"no\".");
        return 1;
    }

    if(member_array(upper_case(which),keywords) != -1){
        which = upper_case(which);
    }
    else switch(lower_case(which)){
        case "autowiz" : which = "AUTO_WIZ";break;
        case "locked" : which = "MUD_IS_LOCKED";break;
        case "localtime" : which = "LOCAL_TIME";break;
        case "justenglish" : which = "ENGLISH_ONLY";break;
        case "justhumans" : which = "HUMANS_ONLY";break;
        case "encumbrance" : which = "ENABLE_ENCUMBRANCE";break;
        case "pk" : which = "PLAYER_KILL";break;
        case "compat"  : which = "COMPAT_MODE";break;
        case "retain" : which = "RETAIN_ON_QUIT";break;
        case "defaultparse" : which = "DEFAULT_PARSING";break;
        case "disablereboot" : which = "DISABLE_REBOOTS";break;
        case "exitsbare" : which = "BARE_EXITS";break;
        case "nmexits" : which = "NM_STYLE_EXITS";break;
        case "matchcommand" : which = "COMMAND_MATCHING";break;
        case "matchobject" : which = "OBJECT_MATCHING";break;
        case "catchtell" : which = "NPC_CATCH_TELL_DEBUG";break;
        case "loglocal" : which = "LOG_LOCAL_CHANS";break;
        case "logremote" : which = "LOG_REMOTE_CHANS";break;
        case "imc2" : which = "DISABLE_IMC2";break;
        case "fastcombat" : which = "FAST_COMBAT";break;
        case "channelpipes" : which = "CHANNEL_PIPES";break;
        case "cgi" : which = "ENABLE_CGI";break;
        case "dirlist" : which = "WWW_DIR_LIST";break;
        case "creweb" : which = "ENABLE_CREWEB";break;
        case "selectclass" : which = "CLASS_SELECTION";break;
        case "severable" : which = "SEVERABLE_LIMBS";break;
        case "questrequired" : which = "REQUIRE_QUESTING";break;
        case "autoadvance" : which = "AUTO_ADVANCE";break;
        case "ced" : which = "CED_DISABLED";break;
        case "minimap" : which = "MINIMAP";break;
        case "wizmap" : which = "WIZMAP";break;
        case "grid" : which = "GRID";break;
        case "guestallowed" : which = "GUEST_ALLOWED";break;
        case "playerintertell" : which = "PLAYER_INTERTELL_ALLOWED";break;
        case "guest" : which = "GUEST_ALLOWED";break;
        default : break;
    }
    foreach(string element in config){
        if(grepp(element, which)){
            string s1, s2, s3;
            if(sscanf(element,"#define %s %s",s1,s2) != 2){
                write("Major problem. You should revert to a backup of "+
                        CONFIG_H+" immediately.");
                return 1;
            }
            s3 = trim(s2);
            s2 = replace_string(s2,s3,arg);
            element = "#define "+s1+" "+s2;
        }
        config2 += ({ element });
    }
    CompleteConfig();
    if(which == "DEFAULT_PARSING" || which == "ENABLE_ENCUMBRANCE" ||
            which == "BARE_EXITS" || which == "COMMAND_MATCHING" ||
            which == "AUTO_ADVANCE"){
        reload(LIB_CREATOR,1,1);
        write("This configuration will take effect for each user the next time they log in.");
        return 1;
    }
    if(which == "NPC_CATCH_TELL_DEBUG"){
        if(file_exists("/domains/default/room/catchtell"))
            reload("/domains/default/room/catchtell");
        return 1;
    }
    if(which == "CED_DISABLED"){
        reload(CMD_CED,1,1);
        reload(LIB_CREATOR,1,1);
        write("This configuration will take effect for each user "+
                "the next time they log in.");
    }
    if(which == "NM_STYLE_EXITS"){
        reload(LIB_ROOM,1,1);
        reload(LIB_CREATOR,1,1);
        write("This configuration will take effect for rooms not yet loaded for each "
                "user the next time they log in. To ensure all rooms pick up the new configuration, "
                "either reboot the mud, or type: \"reload every room\", then quit and log back in.");
    }
    if(which == "RETAIN_ON_QUIT" || which == "OBJECT_MATCHING") 
        write("To make this configuration take effect, reboot the mud.");
    if(which == "LOG_LOCAL_CHANS" || which == "LOG_REMOTE_CHANS"){
        reload(CHAT_D,1,1);
    }
    if(which == "DISABLE_IMC2"){
        object ob;
        IMC2_D->UnSetAutoDisabled(1);
        ob = find_object(IMC2_D);
        if(!junk && ob){
            reload(ob, 0, 1);         
            reload(find_object(CHAT_D), 0, 1);
        }
        else IMC2_D->remove();   
    }
    if(which == "FAST_COMBAT"){
        reload(LIB_CREATOR,1,1);
        write("This configuration will take effect for each user the next time they log in.");
    }

    if(which == "ENABLE_CGI" || which == "WWW_DIR_LIST" || which == "ENABLE_CREWEB"){
        reload(WEB_SESSIONS_D,1,1);
        reload(SOCKET_HTTP,1,1);
        foreach(string element in get_dir(DIR_WWW_GATEWAYS+"/")){
            if(last(element,2) == ".c") reload(DIR_WWW_GATEWAYS+"/"+element,1,1);
        }
    }
    return 1;
}

int ProcessService(string which, string what){
    int port_offset, type, port;
    string sclass;
    if(sscanf(what,"port %d",port)){
        what = "port";
    }
    switch(which){
        case "hftp": port_offset=OFFSET_HFTP;sclass="/secure/lib/net/h_ftpd";type=1;break;
        case "ftp": port_offset=OFFSET_FTP;sclass="/secure/lib/net/ftp";type=1;break;
        case "http": port_offset=OFFSET_HTTP;sclass="/secure/lib/net/http";type=3;break;
        case "rcp": port_offset=OFFSET_RCP;sclass="/secure/lib/net/remote";type=1;break;
        case "oob": port_offset=OFFSET_OOB;sclass="/secure/lib/net/oob";type=0;break;
    }
    switch(what){
        case "add": INET_D->AddService(which,port_offset, sclass, type);break;
        case "remove": INET_D->RemoveService(which);break;
        case "start": INET_D->eventStartServer(which);break;
        case "restart": INET_D->eventRestartServer(which,1);break;
        case "stop": INET_D->eventStopServer(which);break;
        case "port": ModPort(which, port);break;
    }
    if(which == "oob"){
        if( what == "start" || what == "restart")
            reload(OOB_D);
        if( what == "stop" )
            OOB_D->eventDestruct();
    }
    write("Done.");
    return 1;
}

int ProcessInet(string which, string arg){
    int sub;
    string preloads = read_file(CFG_PRELOAD);
    string *load_lines = explode(preloads,"\n");
    string *ret_arr = ({});
    string yesline = "/secure/daemon/inet";
    string noline = "#/secure/daemon/inet";
    validate();
    if(!arg) arg = "status";
    if(which != "inet"){
        sub = 1;
        if(!find_object(INET_D)){
            write("The inet service is not running. Please type: mudconfig inet start");
            write("Or: mudconfig inet enable");
            write("Then retry your command.");
            return 1;
        }
    }
    else {
        if(member_array(arg,yesbools) != -1){
            if(member_array(yesline,load_lines) != -1) {
                write("Persistent inet activation is already enabled.");
                return 1;
            }
            else {
                write("Enabling persistent inet activation.");
                foreach(string line in load_lines){
                    if(line == noline) line = yesline;
                    ret_arr += ({ line });
                }
                if(member_array(yesline,ret_arr) == -1){
                    ret_arr += ({ yesline });
                }
                unguarded( (: cp(CFG_PRELOAD,"/secure/save/backup/preload."+time()) :) );
                ret_string = implode(ret_arr,"\n")+"\n";
                ret_string = replace_string(ret_string,"\n\n","\n");
                unguarded( (: write_file(CFG_PRELOAD,ret_string,1) :) );
                write("The inet service is enabled.");
                arg = "start";
            }
        }

        if(member_array(arg,nobools) != -1){
            if(member_array(noline,load_lines) != -1){
                write("Persistent inet activation is already disabled.");
                return 1;
            }
            else {
                write("Disabling persistent inet activation.");
                foreach(string line in load_lines){

                    if(line == yesline) line = noline;
                    ret_arr += ({ line });
                }
                if(member_array(noline,ret_arr) == -1){
                    ret_arr += ({ noline });
                }
                unguarded( (: cp(CFG_PRELOAD,"/secure/save/backup/preload."+time()) :) );
                ret_string = implode(ret_arr,"\n")+"\n";
                ret_string = replace_string(ret_string,"\n\n","\n");
                unguarded( (: write_file(CFG_PRELOAD,ret_string,1) :) );
                write("The inet service is disabled.");
                arg = "stop";
            }
        }

        if(arg == "start"){
            if(find_object(INET_D)) write("The inet service is already running.");
            else {
                write("Starting the inet service.");
                load_object(INET_D);
            }
        }
        else if(arg == "stop"){
            if(!find_object(INET_D)) write("The inet service is already stopped.");
            else {
                write("Stopping the inet service.");
                find_object(INET_D)->eventDestruct();
            }
        }
        else if(arg == "restart"){
            if(find_object(INET_D)) {
                write("The inet service is running. Stopping it now...");
                find_object(INET_D)->eventDestruct();
            }
            write("Starting the inet service.");
            load_object(INET_D);
        }
        else if(arg == "status"){
            if(member_array(yesline,load_lines) != -1){
                write("Persistent inet activation is enabled.");
            }
            else {
                write("Persistent inet activation is disabled.");
            }

            if(!find_object(INET_D)){
                write("The inet service is not running");
            }
            else{
                string *servkeys, *servkeys2;
                string subret = "The following services are available: "; 
                string subret2 = "The following services are running: "; 
                write("The inet service is running");
                if(sizeof(servkeys = keys(INET_D->GetServices()))){
                    subret += implode(servkeys,", ")+".\n";
                }
                else subret = "";
                if(sizeof(servkeys2 = keys(INET_D->GetServers()))){
                    subret2 += implode(servkeys2,", ")+".\n";
                }
                else subret2 = "";
                write(subret+subret2);
                return 1;
            }
            ret_string = "";
            write("Done.");
            return 1;
        }
        else write("Unsupported inet subcommand.");
        return 1;
    }
    if(arg == "enable"){
        if(INET_D->GetService(which)){
            write("The "+which+" service is already enabled. Perhaps you mean to start or restart it?");
            return 1;
        }
        ProcessService(which,"add");
        return 1;
    }
    if(arg == "disable"){
        if(!(INET_D->GetService(which))){
            write("The "+which+" service is already disabled.");
            return 1;
        }
        ProcessService(which,"remove");
        return 1;
    }

    if(arg == "start"){
        if(INET_D->GetServer(which)){
            write("The "+which+" service is already started. Perhaps you mean to restart it?");
            return 1;
        }
        if(!(INET_D->GetService(which))){
            write("The "+which+" service has been disabled or is not available.");
            return 1;
        }
        ProcessService(which,"start");
        return 1;
    }
    if(arg == "stop"){
        if(!(INET_D->GetService(which))){
            write("The "+which+" service is already disabled and therefore not running.");
            return 1;
        }
        if(!(INET_D->GetServer(which))){
            write("The "+which+" service is already stopped.");
            return 1;
        }
        ProcessService(which,"stop");
        return 1;
    }

    if(arg == "restart"){
        ProcessService(which,"restart");
        return 1;
    }
    if(arg == "status"){
        write("The "+which+" service is "+(INET_D->GetService(which) ? "enabled." : "disabled."));
        if(INET_D->GetService(which))
            write("The "+which+" service is "+(INET_D->GetServer(which) ? "running." : "stopped."));
        return 1;
    }
    if(!strsrch(arg,"port ")){
        ProcessService(which,arg);
        return 1;
    }
    write("Unsupported mudconfig inet service subcommand.");
    return 1;
}

varargs static int ModCfg(string which, string arg){
    int port, oldport, newport, ret, justbooted;
    string *line_array;
    string mconfig, nameline, portline, newline, newfile;
    string line_string, junk, name;
    object i3, imc2;

    justbooted = uptime() - 62;
    if(justbooted < -1){
        write("Please wait "+abs(justbooted)+" seconds and try again.");
        return 1;
    }

    if(!find_object(INSTANCES_D) || !ENABLE_INSTANCES ||
            INSTANCES_D->GetMyInstanceName() == "global"){
        port = 0;
        mconfig = "/secure/cfg/mudos.cfg";
    }
    else {
        port = query_host_port();
        mconfig = "/secure/cfg/mudos."+port+".cfg";
    }
    line_string = read_file(mconfig);
    if(!sizeof(line_string)) write("Couldn't read file.");
    line_array = explode(line_string, "\n");
    if(!sizeof(line_array)) write("Array is zero length.");

    if(!sizeof(line_array) || !sizeof(line_string)) {
        return 1;
    }

    foreach(string line in line_array){
        if(!strsrch(line,"name :")){
            nameline = line;
            if(which == "name") break;
        }
        if(!strsrch(line,"external_port_")){
            portline = line;
            if(which == "port"){
                newport = atoi(arg);
                break;
            }
        }
    }

    if(which == "name"){
        if(!nameline || sscanf(nameline,"%s : %s",junk, name) < 2) {
            write("Operation failed. You need to copy over "+
                    mconfig+" immediately with an original.");
            return 1;
        }

        newline = junk + " : " + arg;
        newfile = replace_string(line_string, nameline, newline);
        write_file(mconfig,newfile,1);
        cp(mconfig,"/secure/cfg/mudos.autobak."+query_host_port());
        if(query_windows()){
            if(port){
                cp(mconfig,"/secure/cfg/mudos."+port+".win32");
            }
            else {
                cp(mconfig,"/secure/cfg/mudos.win32");
            }
        }
        imc2 = find_object(IMC2_D);
        i3 = find_object(INTERMUD_D);
        if(imc2) imc2->eventDestruct();
        if(i3) i3->eventDestruct();
        if(imc2) destruct(imc2);
        if(i3) destruct(i3);
        ret = MASTER_D->SetMudName(arg);
        write("\n");
        write("\nMUD's name is now: "+mud_name());
        if(!DISABLE_IMC2){
            write("Reloading IMC2...");
            catch(reload(IMC2_D, 0, 1));
        }
        if(!DISABLE_INTERMUD){
            write("Reloading Intermud-3...");
            catch(reload(INTERMUD_D, 0, 1));
        }
        catch(reload(PING_D, 0, 1));
    }

    if(which == "port"){
        if(!portline || sscanf(portline,"%s : telnet %d",junk, oldport) < 2){
            write("Operation failed. You need to copy over "+
                    "/secure/cfg/mudos.cfg immediately with an original.");
            return 1;
        }

        newline = junk + " : telnet " + newport;
        newfile = replace_string(line_string, portline, newline);
        write_file("/secure/cfg/mudos.cfg",newfile,1);
        cp(mconfig,"/secure/cfg/mudos.autobak."+query_host_port());
        if(query_windows()){
            if(port){
                cp(mconfig,"/secure/cfg/mudos."+newport+".win32");
            }
            else {
                cp(mconfig,"/secure/cfg/mudos.win32");
            }
        }
        write("\nMUD's port changed. Reboot the MUD to activate new port.");
        write("NOTE: If the port you selected is 1024 or below, your OS "+
                "may require the MUD to run as a privileged user.");
    }
    return 1;
}

string GetHelp(){
    return ("Syntax: mudconfig PARAMETER VALUE \n\n"
            "Modifies various system settings.\n"
            "Examples: \n"
            "\nmudconfig autowiz [ yes | no ]"
            "\nmudconfig locked [ yes | no ]"
            "\nmudconfig justenglish [ yes | no ]"
            "\nmudconfig justhumans [ yes | no ]"
            "\nmudconfig encumbrance [ yes | no ]"
            "\nmudconfig severable [ yes | no ] (whether limbs can be "
        "severed in combat. Requires a warmboot.)"
            "\nmudconfig pk [ yes | no ]"
            "\nmudconfig minimap [ yes | no ] (whether players get a minimap)"
            "\nmudconfig wizmap [ yes | no ] (whether cres get an area map)"
            "\nmudconfig grid [ yes | no ] (enable or disable the room grid "
        "system)"
            "\nmudconfig compat [ yes | no ]"
            "\nmudconfig retain [ yes | no ]"
            "\nmudconfig defaultparse [ yes | no ]"
            "\nmudconfig disablereboot [ yes | no ]"
            "\nmudconfig matchcommand [ yes | no ]"
            "\nmudconfig matchobject [ yes | no ]"
            "\nmudconfig exitsbare [ yes | no ]"
            "\nmudconfig nmexits [ yes | no ] (This togggles where default "
            "exits are displayed)"
            "\nmudconfig fastcombat [ yes | no ] (heart rate overridden "
            "in combat)"
            "\nmudconfig selectclass [ yes | no ] (whether new players "
            "choose a class on login)"
            "\nmudconfig instances [ yes | no ] (whether mud instances "
            "are used)"
            "\nmudconfig localtime [ yes | no ]"
            "\nmudconfig offset <offset from gmt in seconds>"
            "\nmudconfig extraoffset <offset from GMT in hours>"
            "\nmudconfig maxcommands <max number of commands per second>"
            "\nmudconfig maxidle <number of idle seconds before autoquit>"
            "\nmudconfig questrequired [ yes | no ]"
            "\nmudconfig autoadvance [ yes | no ]"
            "\nmudconfig guestallowed [ yes | no ]"
            "\nmudconfig playerintertell [ yes | no ]"
            "\nmudconfig ced [ yes | no ] (toggles the fullscreen editor)"
            "\nmudconfig maxip <max connections per IP>"
            "\nmudconfig pinginterval <i3 ping interval in seconds>"
            "\nmudconfig monitor <monitoring level, 0 to 2>"
            "\nmudconfig newbielevel <max newbie level>"
            "\nmudconfig resets <interval between resets>"
            "\nmudconfig router [ enable | disable ]"
            "\nmudconfig startroom <filename of start room>"
            "\nmudconfig defaultdomain </full/path>"
            "\nmudconfig email <the admin's email address>"
            "\nmudconfig liveupgrade <the default liveupgrade mud's name>"
            "\nmudconfig hostip <the computer's ip address "
            "(eg 111.222.333.444)>"
            "\nmudconfig websourceip <the remote web server's ip address "
            "(eg 111.222.333.444)>"
            "\nmudconfig websourcename <the remote web server's ip name "
            "(eg a.b.com)>"
            "\nmudconfig channelpipes [ enable | disable ] (whether to allow "
            "piping messages. not recommended.)"
            "\nmudconfig intermud [ enable | disable | restrict | "
            "unrestrict | reset ]"
            "\nmudconfig imc2 [ enable | disable ]"
            "\nmudconfig imc2clientpass <client password for IMC2>"
            "\nmudconfig imc2serverpass <server password for IMC2>"
            "\nmudconfig inet [ enable | disable | start | stop | restart "
            "| status ]"
            "\nmudconfig ftp [ enable | disable | start | stop | restart "
            "| status ]"
            "\nmudconfig hftp [ enable | disable | start | stop | restart "
            "| status ]"
            "\nmudconfig rcp [ enable | disable | start | stop | restart "
            "| status ]"
            "\nmudconfig oob [ enable | disable | start | stop | restart "
            "| status ]"
            "\nmudconfig http [ enable | disable | start | stop | restart "
            "| status ]"
            "\nmudconfig cgi [ enable | disable ] (Whether the mud webserver "
            "should use CGI)"
            "\nmudconfig dirlist [ enable | disable ] (Allow the webserver "
            "to display dir contents)"
            "\nmudconfig creweb [ enable | disable ] (Allow web based "
            "editing [requires cgi and dirlist])"
            "\nmudconfig loglocal [ enable | disable ] (whether local "
            "channels are logged)"
            "\nmudconfig logremote [ enable | disable ] (whether remote "
            "channels are logged)"
            "\nmudconfig mudname <name>"
            "\nmudconfig mudport <port>"
            "\n\nSee also: admintool, config");
}

