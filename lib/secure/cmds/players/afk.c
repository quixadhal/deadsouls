/* /secure/cmds/player/afk.c
 *  From the Frontiers LPC library
 *  created by Cratylus, 17Nov96
 */

#include <lib.h>

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
    return 1;
}

void help() {
    message("help", "Syntax: <afk>\n\n"
            "Toggles your 'away from keyboard' flag.\n\n", this_player());
}
