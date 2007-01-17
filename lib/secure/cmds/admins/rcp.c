#include <lib.h>
#include <dirs.h>
#include <cfg.h>
#include <daemons.h>

inherit LIB_DAEMON;

string globalfile, globalmud;
string array allnames = ({});
int i = 0;

mixed cmd( string str) {
    string mud, file;
    int foo;
    mapping NewFiles = ([]);
    globalmud = "";
    globalfile = "";
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
	    NewFiles[element] = replace_string(replace_string(element,"\\","/"),
	      DIR_UPGRADES_FILES+"/","");
	    rename(element, NewFiles[element]);
	}
	write("Done.");
	return 1;
    }

    globalmud = mud;
    if(file == "all"){
	string tmp = replace_string(DIR_UPGRADES_TXT+"/upgrades.txt","/","\\");
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
    write("Single file upgrade begun.");
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
