#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

int cmd(string str) {
    object ob;

    if( !str || str == "" ) {
        str = "all";
    }
    else if(!(ob=find_player(str=lower_case(str))) && str != "all"){
        write(str+": no such player.\n");
        return 1;
    }
    SNOOP_D->RemoveWatcher(this_player()->GetKeyName(), str);
    write("The snoop daemon has received your request.");
    return 1;
}

string GetHelp(){
    return "Syntax: unsnoop <user>\n\n"
        "Stops snooping the specified user.\n"
        "See also: snoop, monitor, unmonitor";
}
