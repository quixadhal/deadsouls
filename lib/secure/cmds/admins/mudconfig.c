#include <lib.h>
#include <cfg.h>
#include <save.h>
#include <daemons.h>
#include <network.h>
void help();

inherit LIB_DAEMON;

string ret_string = "";
string array config, config2, keywords;
string array bools = ({ "enable","disable","on","off","1","0" });
string array yesbools = ({ "enable","on","1","yes" });
string array nobools = ({ "disable","off","0","no" });
string array restrict_tokens = ({ "restrict","unrestrict" });
string array nonmodals = ({ "liveupgrade", "prompt","status","email","websource",
  "debugger", "access", "pinging" });
string array modals = ({ "matchcommand", "matchobject", "autowiz", "locked","localtime", 
  "justenglish", "justhumans", "encumbrance", "pk", "compat", "exitsbare", "nmexits",
  "retain", "defaultparse", "disablereboot" });
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

static private void validate() {
    if(!this_player()) return 0;
    if( !((int)master()->valid_apply(({ "ASSIST" }))) )
	error("Illegal attempt to access mudconfig: "+get_stack()+" "+identify(previous_object(-1)));
}

mixed cmd(string str) {
    string which, arg;

    validate();

    if(!str || sscanf(str, "%s %s", which, arg) != 2){
	help();
	return 1;
    }
    cp("/secure/include/config.h","/secure/save/backup/config."+time());
    config = explode( read_file("/secure/include/config.h"),"\n" );
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
    case "maxcommands" : which = "MAX_COMMANDS_PER_SECOND";ProcessOther(which,arg);break;
    case "maxidle" : which = "IDLE_TIMEOUT";ProcessOther(which,arg);break;
    case "hostip" : which = "HOST_IP";ProcessString(which,arg);break;
    case "email" : which = "ADMIN_EMAIL";ProcessString(which,arg);break;
    case "liveupgrade" : which = "LIVEUPGRADE_SERVER";ProcessString(which,arg);break;
    case "mudstatus" : which = "MUD_STATUS";ProcessString(which,arg);break;
    case "debugger" : which = "DEBUGGER";ProcessString(which,arg);break;
    case "websource" : which = "WEB_SOURCE";ProcessString(which,arg);break;
    default : NotImplemented(which);break;
    }
    return 1;
}

varargs static int CompleteConfig(string file){
    string ret = implode(config2,"\n")+"\n";
    if(!file) file = "/secure/include/config.h";
    validate();
    ret = replace_string(ret,"\n\n","\n");
    write_file(file,ret,1);
    reload(MASTER_D,0,1);
    reload(LIB_CONNECT,0,1);
    write("Command complete.");
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
    cp("/secure/include/rooms.h","/secure/save/backup/rooms."+time());
    config = explode( read_file("/secure/include/rooms.h"),"\n" );
    config2 = ({});
    foreach(string line in config){
	string s1,s2,s3;
	if(sscanf(line,"%s %s %s",s1,s2,s3) == 3){
	    if(s1 == "#define" && s2 == "ROOM_START") 
		line = "#define ROOM_START       \""+arg+"\"";
	}
	config2 += ({ line });
    }
    CompleteConfig("/secure/include/rooms.h");
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
	return 1;
    }

    if(arg == "reset"){
	object ob = find_object(INTERMUD_D);
	write("Purging all intermud data (including router password!). Previous data file saved to /secure/save/backup/ .");
	if(!ob){
	    if(file_exists(SAVE_INTERMUD+".o")) rename(SAVE_INTERMUD+".o", "/secure/save/backup/intermud."+time());
	}
	else {
	    if(file_exists(SAVE_INTERMUD+".o")) rename(SAVE_INTERMUD+".o", "/secure/save/backup/intermud."+time());
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
	flush_messages(this_player());
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
		write("Major problem. You should revert to a backup of config.h immediately.");
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
	reload("/daemon/time",0,1);
	reload("/cmds/creators/people",0,1);
	reload("/cmds/players/date",0,1);
	reload("/cmds/players/nextreboot",0,1);
	reload("/cmds/players/version",0,1);
	write("This configuration change will require a few minutes to take effect completely.");
    }
    if(which == "GLOBAL_MONITOR") reload(SNOOP_D,0,1);
    if(which == "IDLE_TIMEOUT" || which == "MAX_NEWBIE_LEVEL"){ 
	reload(LIB_CREATOR,1,1);
	write("This configuration will take effect for each user the next time they log in.");
	return 1;
    }
    return 1;
}

static int ProcessString(string which, string arg){
    object ob;
    validate();

    foreach(string element in config){
	if(grepp(element, which)){
	    string s1, s2, s3;
	    if(sscanf(element,"#define %s %s",s1,s2) != 2){
		write("Major problem. You should revert to a backup of config.h immediately.");
		return 1;
	    }
	    s3 = trim(s2);
	    s2 = replace_string(s2,s3,"\""+arg+"\"");
	    element = "#define "+s1+" "+s2;
	}
	config2 += ({ element });
    }
    if(which == "WEB_SOURCE" && ob = find_object("/secure/cmds/admins/liveupgrade")){
	ob->eventDestruct();
    }
    CompleteConfig();
    return 1;
}

static int ProcessModal(string which, string arg){
    int junk;
    validate();
    if(member_array(arg,yesbools) != -1) arg = "1";
    if(member_array(arg,nobools) != -1) arg = "0";
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
    default : break;
    }
    foreach(string element in config){
	if(grepp(element, which)){
	    string s1, s2, s3;
	    if(sscanf(element,"#define %s %s",s1,s2) != 2){
		write("Major problem. You should revert to a backup of config.h immediately.");
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
      which == "BARE_EXITS" || which == "COMMAND_MATCHING"){ 
	reload(LIB_CREATOR,1,1);
	write("This configuration will take effect for each user the next time they log in.");
	return 1;
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
    return 1;
}

int ProcessService(string which, string what){
    int port_offset, type;
    string sclass;
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
    write("Unsupported mudconfig inet service subcommand.");
    return 1;
}

void help() {
    message("help",
      "Syntax: mudconfig PARAMETER VALUE \n\n"
      "Modifies various system settings.\n"
      "Examples: \n"
      "\nmudconfig autowiz [ yes | no ]"
      "\nmudconfig locked [ yes | no ]"
      "\nmudconfig justenglish [ yes | no ]"
      "\nmudconfig justhumans [ yes | no ]"
      "\nmudconfig encumbrance [ yes | no ]"
      "\nmudconfig pk [ yes | no ]"
      "\nmudconfig compat [ yes | no ]"
      "\nmudconfig retain [ yes | no ]"
      "\nmudconfig defaultparse [ yes | no ]"
      "\nmudconfig disablereboot [ yes | no ]"
      "\nmudconfig matchcommand [ yes | no ]"
      "\nmudconfig matchobject [ yes | no ]"
      "\nmudconfig exitsbare [ yes | no ]"
      "\nmudconfig nmexits [ yes | no ] (This togggles where default exits are displayed)"
      "\nmudconfig localtime [ yes | no ]"
      "\nmudconfig offset <offset from gmt in seconds>"
      "\nmudconfig extraoffset <offset from GMT in hours>"
      "\nmudconfig maxcommands <max number of commands per second>"
      "\nmudconfig maxip <max connections per IP>"
      "\nmudconfig monitor <monitoring level, 0 to 2>"
      "\nmudconfig newbielevel <max newbie level>"
      "\nmudconfig resets <interval between resets>"
      "\nmudconfig router [ enable | disable ]"
      "\nmudconfig startroom <filename of start room>"
      "\nmudconfig defaultdomain </full/path>"
      "\nmudconfig email <the admin's email address>"
      "\nmudconfig liveupgrade <the default liveupgrade mud's name>"
      "\nmudconfig hostip <the computer's ip address (eg 111.222.333.444)>"
      "\nmudconfig websource <the remote web server's ip address (eg 111.222.333.444)>"
      "\nmudconfig intermud [ enable | disable | restrict | unrestrict | reset ]"
      "\nmudconfig inet [ enable | disable | start | stop | restart | status ]"
      "\nmudconfig ftp [ enable | disable | start | stop | restart | status ]"
      "\nmudconfig hftp [ enable | disable | start | stop | restart | status ]"
      "\nmudconfig http [ enable | disable | start | stop | restart | status ]"
      "\nmudconfig rcp [ enable | disable | start | stop | restart | status ]"
      "\nmudconfig oob [ enable | disable | start | stop | restart | status ]"
      "\n\nSee also: admintool", this_player()
    );
}
