#include <lib.h>
#include <cfg.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string *races = get_dir(CFG_RACES+"/");
    foreach(string race in races){
	string str = CFG_RACES+"/"+race;
	if(file_exists(str)) {
	    if(race == "human") RACES_D->AddRace(str, 1);
	    else RACES_D->AddRace(str, 0);
	}
    }
    write("Done.");
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <addraces>\n"
      "Imports all the race files in CFG_RACES into  "
      "RACES_D. By default, this command sets humans "
      "as the only player-usable race. If there are other "
      "races (such as your own, if you're not human) that "
      "need to be player-usable, you'll need to use the "
      "removerace command to remove them "
      "and use the addrace command with the proper "
      "options to add them with player-usability.");
}
