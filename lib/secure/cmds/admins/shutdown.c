//      /bin/adm/_shutdown.c
//      from the Dead Souls Mudlib
//      the shutdown command
//      created in pats by Sulam@TMI, Plura@Dead Souls, and Descartes of Borg

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string str) {
    if(this_player()->GetForced()) return 0;
    if(!archp(previous_object())) {
	    notify_fail("You are not permitted to shutdown the game.\n");
	    return 0;
    }
    if(!str) {
	notify_fail("You must give a shutdown reason as argument.\n");
	return 0;
    }
    shout("Game is shut down by " + this_player()->GetKeyName() + ".\n");
    log_file("game_log", ctime(time())+" Game shutdown by "+
	this_player()->GetKeyName()+"("+str+")\n");
    shutdown();
    return 1;
}

void help() {
    write("Syntax: <shutdown [reason]>\n\n"
      "This will shut down the game immediately.\n\nSee also:\nend\n"
    );
}
