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
        write("You can't pause yourself.");
        return 1;
    }
    write("You pause "+capitalize(args)+".");
    ob->SetPlayerPaused(1);
    ob->eventPrint("You have been paused. Commands will not work until you are unpaused.");
    return 1;
}

string GetHelp(string args) {
    return ("Syntax: pause <PLAYER>\n\n"
            "Causes the player to be unable to run any commands. Useful for "
            "immediately halting problem behavior.\n\n"
            "");
}
