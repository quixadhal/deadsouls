/* /secure/cmds/player/afk.c
 *  From the Frontiers LPC library
 *  created by Cratylus, 17Nov96
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    int afk;
    afk = this_player()->GetProperty("afk");
    if(afk){
        afk = 0;
        message("system", "You are back.", this_player() );
    }
    else {
        afk = 1;
        message("system", "You are away from your keyboard.", this_player() );
    }
    this_player()->SetProperty("afk", afk);
    if(find_object(INSTANCES_D)){
        INSTANCES_D->SendWhoUpdate(this_player()->GetKeyName());
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: afk\n\n"
            "Toggles your 'away from keyboard' flag.");
}
