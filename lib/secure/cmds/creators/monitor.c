#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    object ob;

    if( !str || str == "" ) {
        write("Monitor whom?\n");
    }
    else if(!user_exists(str))
        write(str+": no such player.\n");
    else{
        SNOOP_D->AddMonitor(this_player()->GetKeyName(), str);
        write("The snoop daemon has received your request.");
    }
    return 1;
}

string GetHelp(){
    return "Syntax: monitor <user>\n\n"
        "Logs to a file everything <user> sees or types. "
        "To stop monitoring, an admin must type \"unmonitor <user>\"\n"
        "See also: unmonitor, snoop, unsnoop";
}
