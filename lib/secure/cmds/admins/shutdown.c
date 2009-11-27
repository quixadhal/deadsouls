//      /bin/adm/_shutdown.c
//      from the Dead Souls Mudlib
//      the shutdown command
//      created in pats by Sulam@TMI, Plura@Dead Souls, and Descartes of Borg

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;
object downer;

void ShutDown(){
    shutdown();
}

void DoSaves(){
    object *persists = objects( (: $1->GetPersistent() :) );
    foreach(object persist in persists){
        persist->eventDestruct();
    }
}

int cmd(string str) {
    downer = previous_object();
    if(downer->GetForced()){
        write("Forced attempt to shutdown.");
        return 1;
    }
    if(!archp(downer)) {
        write("You are not permitted to shutdown the game.\n");
        return 1;
    }
    if(!str) {
        write("You must give a shutdown reason as argument.\n");
        return 1;
    }
    if(str == "cancel"){
        int cancel = EVENTS_D->eventCancelShutdown();
        if(cancel == -1){
            write("There was no shutdown in progress.");
        }
        else if(cancel > -1){
            write("Shutdown cancelled.");
            shout("Shutdown cancelled by " + downer->GetKeyName() + ".\n");
        }
        else {
            write("Something odd happened. Shutdown may not be cancelled.");
        }
        return 1;
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
    call_out( (: DoSaves :), 0);
    call_out( (: shutdown :), 2);
    return 1;
}

string GetHelp(){
    return ("Syntax:  shutdown cancel\n"
            "         shutdown [reason]\n\n"
            "This will shut down the game immediately, or cancel "
            "a scheduled shutdown. Use with great care.\nSee also: end");
}
