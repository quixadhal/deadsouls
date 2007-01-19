#include <lib.h>
#include <dirs.h>
#include <cfg.h>
#include <daemons.h>
#include <network.h>

inherit LIB_DAEMON;

string array allnames;
int i;

mixed cmd( string str) {
    string mud, file;
    int foo;
    mapping NewFiles = ([]);
    object inet = find_object(INET_D);
    string *preload_file = explode(read_file(CFG_PRELOAD),"\n");
    i = 0;
    allnames = ({});
    if(!this_player()) return 0;
    if( !((int)master()->valid_apply(({ "SECURE" }))) )
	error("Illegal attempt to access liveupgrade: "+get_stack()+" "+identify(previous_object(-1)));

    foo = sscanf(str,"%s %s",file, mud);

    if(str == "apply"){
	string *files = ({});
	foreach(string element in get_dir(DIR_UPGRADES_FILES+"/")){
	    files += ({ DIR_UPGRADES_FILES+"/"+element });
	}
	foreach(string element in files){
	    NewFiles[element] = replace_string(replace_string(element,"0^0","/"),
	      DIR_UPGRADES_FILES+"/","");
	    load_object("/secure/cmds/creators/bk")->cmd(NewFiles[element]);
	    rename(element, NewFiles[element]);
	}
	if(member_array(INET_D,preload_file) == -1 && inet) inet->eventDestruct();
	write("Done.");
	return 1;
    }

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
    if(file == "all"){
	string tmp = replace_string(DIR_UPGRADES_TXT+"/upgrades.txt","/","0^0");
	OOB_D->GetFile(mud,DIR_UPGRADES_TXT+"/upgrades.txt");
	if(!file_exists(DIR_UPGRADES_TXT+"/list.txt")){
	    write("You're either missing the current updates file, or it is "
	      "still downloading. Please wait a few moments then try again.");
	    return 1;
	}

	allnames = explode(read_file(DIR_UPGRADES_TXT"/list.txt"),"\n");

	OOB_D->eventMajorUpgrade(mud, allnames);
	write("Full upgrade begun.");
	return 1;
    }
    OOB_D->GetFile(mud,file);
    write("Requesting the file \""+file+"\" from "+INTERMUD_D->GetMudName(mud)+".");
    return 1;
}

string GetHelp() {
    return ("Syntax: liveupgrade [<filename> | all] [SERVER]\n"
      "        liveupgrade apply\n"
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
