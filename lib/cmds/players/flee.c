#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    if(!(this_player()->GetInCombat())){
        write("You are not in combat.");
        return 1;
    }
    this_player()->eventWimpy("go out");
    return 1;
}

string GetHelp() {
    return ("Syntax: flee\n\n"
            "If you need to leave combat in a hurry, and don't care which way you go.\n"
            "This command doesn't work for creators.");
}
