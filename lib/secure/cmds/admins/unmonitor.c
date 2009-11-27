#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    object ob;

    if(!archp(previous_object())) {
        write("Only an arch may halt a monitoring process.");
        return 0;
    }
    if( !str || str == "" ) {
        write("Unmonitor whom?\n");
    }
    else if(!user_exists(str))
        write(str+": no such player.\n");
    else{
        SNOOP_D->RemoveMonitor(this_player(), str);
        write("The snoop daemon has received your request.");
    }
    return 1;
}

string GetHelp(){
    return "Syntax: unmonitor <user>\n\n"
        "Stops the logging of a user's input and output.\n"
        "See also: monitor, snoop, unsnoop";
}
