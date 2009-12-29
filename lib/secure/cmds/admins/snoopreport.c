#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string ret;

    if(!this_player() || !archp(this_player())) return 0;

    ret = SNOOP_D->Report();

    if(check_string_length(ret) > 0) this_player()->eventPrint(""+ret+"");
    else print_long_string(this_player(),ret);

    return 1;
}

string GetHelp() {
    return ("Syntax: snoopreport\n\n"
            "Displays a list of who is snooping whom, who is being "
            "monitored (aka logged), and so on.\nSee also: snoop, unsnoop, "
            "monitor");
}
