#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object ob;
    if(!ob=find_player(args)){
        write("Unable to find that player.");
        return 1;
    }
    if(ob == this_player()){
        write("You can't unpause yourself.");
        return 1;
    }
    write("You unpause "+capitalize(args)+".");
    ob->SetPlayerPaused(0);
    ob->eventPrint("You have been unpaused. Commands will work.");
    return 1;
}

string GetHelp(string args) {
    return ("Syntax: unpause <PLAYER>\n\n"
            "Causes the player to be able to run commands after being paused.\n\n"
            "");
}
