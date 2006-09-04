//      /bin/adm/_shutdown.c
//      from the Dead Souls Mudlib
//      the shutdown command
//      created in pats by Sulam@TMI, Plura@Dead Souls, and Descartes of Borg

#include <lib.h>

inherit LIB_DAEMON;
object downer;

void ShutDown(){
    shutdown();
}

int cmd(string str) {
    downer = previous_object();
    tc("downer: "+identify(downer));
    tc("downers: "+identify(previous_object(-1)));
    tc("stack: "+get_stack());
    if(downer->GetForced()){
     write("Forced attempt to shutdown.");
     return 0;
     }
    if(!archp(downer)) {
	write("You are not permitted to shutdown the game.\n");
	return 0;
    }
    if(!str) {
	write("You must give a shutdown reason as argument.\n");
	return 0;
    }
    shout("Game is shut down by " + downer->GetKeyName() + ".\n");
    log_file("game_log", ctime(time())+" Game shutdown by "+
      downer->GetKeyName()+"("+str+")\n");
    foreach(object dude in users()){
	if(dude && sizeof(base_name(dude))){
	    if(!archp(dude)) dude->eventForce("quit");
	    else {
		downer = dude;
		unguarded( (: downer->AddCarriedMass(-99999) :) );
		unguarded( (: downer->save_player(downer->GetKeyName()) :) );
	    }
	}
    }
    call_out( (: shutdown :), 1);
    tc("hm.");
    return 1;
}

void help() {
    write("Syntax: <shutdown [reason]>\n\n"
      "This will shut down the game immediately.\n\nSee also:\nend\n"
    );
}
