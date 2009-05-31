#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    int what;
    if( !sizeof(str) )
        return "Syntax: chancolors <on or off>";
    str = lower_case(str);
    if( str != "on" && str != "off" )
        return "You can only turn chancolors 'on' or 'off'";
    if(str == "on") what = 0;
    else what = 1;
    this_player()->SetNoChanColors(what);
    message("system",
            "Colored channels mode turned "+str+".",
            this_player() );
    return 1;
}

void help() {
    message("help",
            "Syntax: chancolors on\n"
            "        chancolors off\n\n"
            "This command allows you to toggle whether you see colors in channel messages. \nNote: This doesn't apply to the normal colors of the channel name.\n\n",
            this_player() );
}

