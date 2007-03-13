#include <lib.h>
#include <dirs.h>
#include <cfg.h>
#include <daemons.h>
#include <network.h>

#define WGET_D "/secure/daemon/wget"
#define WEB_SOURCE "127.0.0.1"
#define WEB_SOURCE_NAME "dead-souls.net"

inherit LIB_DAEMON;

string array allnames = ({});
string array tmpnames;
string upgrade_prefix = "/code/upgrades/"+mudlib_version();
int i;
int oob = 0;
object player;

mixed cmd( string str) {
    string mud = "Dead Souls"; 
    string file;
    int foo, tmpint = 0;
    mapping NewFiles = ([]);
    object inet = find_object(INET_D);
    string *preload_file = explode(read_file(CFG_PRELOAD),"\n");
    mixed *socks = socket_status();

    foreach(mixed element in socks){
	//tc("1: "+element[1]+", 4: "+element[4]);
	if(element[1] == "DATA_XFER" && element[4] == WEB_SOURCE+".80"){
	    write("A download is still in progress. Please wait until it is complete.");
	    return 1;
	}
    }

    player = this_player();
    i = 0;
    allnames = ({});
    if(!player) return 0;

    if( !((int)master()->valid_apply(({ "SECURE" }))) )
	error("Illegal attempt to access liveupgrade: "+get_stack()+" "+identify(previous_object(-1)));

    if(!directory_exists("/secure/upgrades/bak")){
	mkdir("/secure/upgrades/bak");
    }

    sscanf(mudlib_version(),"2.3a%d",tmpint);
    if(tmpint < 12) oob = 1;
    else {
	if(!strsrch(str,"-o ")){
	    oob = 1;
	    str = replace_string(str,"-o ","");
	}
	else oob = 0;
    }

    foo = sscanf(str,"%s %s",file, mud);
    if(!foo || foo < 2) file = str;

    if(str == "apply"){
	string *files = ({});

	foreach(string element in get_dir(DIR_UPGRADES_FILES+"/")){
	    files += ({ DIR_UPGRADES_FILES+"/"+element });
	}
	foreach(string element in files){
	    string contents = "";
	    NewFiles[element] = replace_string(replace_string(element,"0^0","/"),
	      DIR_UPGRADES_FILES+"/","");
	    contents = read_file(element);
	    if(!contents) contents = "";
	    if(last(contents,1) != "\n") contents += "\n";
	    write_file(element, contents, 1);
	    load_object("/secure/cmds/creators/bk")->cmd(NewFiles[element]);
	    if(directory_exists(NewFiles[element])) rm(element);
	    else rename(element, NewFiles[element]);
	}
	if(member_array(INET_D,preload_file) == -1 && inet) inet->eventDestruct();
	reload(UPDATE_D);
	rm("/secure/upgrades/txt/list.txt");
	write("Done.");
	return 1;
    }

    if(oob){
	tn("oob lu");
	if(!inet){
	    inet = load_object(INET_D);
	    write("Starting INET_D.");
	    if(member_array(INET_D,preload_file) == -1)
		write("When you complete the upgrade by using the \"apply\" keyword, the "
		  "inet daemon will be shut down, since you do not have it enabled by "
		  "default. Please remember to either apply the upgrades when the downloading "
		  "is complete, or manually shut down INET_D with the command: mudconfig inet stop\n");
	}
	if(!inet){
	    write("There is a problem with INET_D. The upgrade will not proceed.");
	    return 1;
	}

	if(!INET_D->GetService("oob")){
	    write("The OOB service is not enabled. Enabling it now.");
	    INET_D->AddService("oob",OFFSET_OOB,LIB_OOB,0);
	}

	if(!INET_D->GetService("oob")){
	    write("There was a problem enabling the OOB service. The upgrade will not proceed.");
	    return 1;
	}

	if(!INET_D->GetServer("oob")){
	    write("The OOB service is not started. Starting it now.");
	    INET_D->eventStartServer("oob");
	}

	if(!INET_D->GetServer("oob")){
	    write("There was a problem starting the OOB service. The upgrade will not proceed.");
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
	    write("That liveupgrade server appears unavailable.");
	    return 1;
	}
    }

    if(file == "all"){
	string tmp = replace_string(DIR_UPGRADES_TXT+"/upgrades.txt","/","0^0");
	tn("lu, all, prefix: "+upgrade_prefix,"green");
	if(!file_exists(DIR_UPGRADES_TXT+"/list.txt")){
	    write("You're either missing the current updates file, or it is "
	      "still downloading. Please wait a minute then try again.");
	    rename("/secure/upgrades/files","/secure/upgrades/bak/"+time());
	    mkdir("/secure/upgrades/files");
	    if(oob){
		OOB_D->GetFile(mud,DIR_UPGRADES_TXT+"/upgrades.txt");
	    }
	    else {
		WGET_D->GetFile(WEB_SOURCE, upgrade_prefix+"/upgrades.txt",WEB_SOURCE_NAME,
		  "/secure/upgrades/txt/list.txt");
	    }
	    return 1;
	}

	tmpnames = explode(read_file(DIR_UPGRADES_TXT"/list.txt"),"\n");
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
	//write("Asking for: "+implode(allnames,", "));
	rm(DIR_UPGRADES_TXT+"/list.txt");
	write("Full upgrade begun.");
	write("Please wait until the netstat command shows no more connections to "+
	  "the upgrade server, then issue the command: liveupgrade apply\n\n");
	write("%^FLASH%^RED%^WARNING! %^BLACK%^WARNING! %^YELLOW%^WARNING! %^RESET%^WARNING!");
	write("You must *always* do a full backup before applying the liveupgrade. "+
	  "If the liveupgrade screwed up, and you get garbage files because of connection "+
	  "problems, it may be necessary for you to restore from backup to be able to "+
	  "start the mud again. You've been warned.");
	return 1;
    }
    if(oob){
	OOB_D->GetFile(mud,file);
	write("Requesting the file \""+file+"\" from "+INTERMUD_D->GetMudName(mud)+".");
    }
    else {
	write("Requesting the file \""+file+"\" from "+WEB_SOURCE);
	WGET_D->GetFile(WEB_SOURCE, upgrade_prefix+file);
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: liveupgrade [-o] [<filename> | all] [SERVER]\n"
      "        liveupgrade apply\n"
      "To use oob updates, use the -o flag. Do not use it to use the default, "
      "which is currently an http connection to dead-souls.net, which is vastly "
      "faster and more secure than oob.\n"
      "To upgrade all files to the current level on the default liveupgrade server:\n"
      "liveupgrade all\n"
      "This will begin the download process. Make sure nobody is masquerading as "
      "your default server! You can specify a liveupgrade sever like this:\n"
      "liveupgrade all Dead Souls\n"
      "Where Dead Souls is a mud on your intermud router that you trust and is up to date. "
      "You can also specify a single file to upgrade, such as: \n"
      "liveupgrade /lib/player.c\n"
      "Wait until there are no more oob callouts before finalizing the upgrade. You "
      "can see pending callouts using the callouts command. If no oob callouts are "
      "pending, you can finalize the upgrade by typing:\n"
      "liveupgrade apply\n"
      "This will delete your old copies of files and copy the newly downloaded "
      "ones in their place. NEVER do a liveupgrade without a full backup first."
      "");
}
