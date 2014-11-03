/*  /cmds/player/brief.c
 *  A command which allows players to toggle brief room descriptions
 *  Created by Blitz at NM-IVD
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    if( !sizeof(str) )
        return "Syntax: brief <on or off>";
    str = lower_case(str);
    if( str != "on" && str != "off" )
        return "You can only turn brief 'on' or 'off'";
    this_player()->SetBriefMode(str == "on");
    message("system",
            "Brief mode turned "+str+".",
            this_player() );
    return 1;
}

string GetHelp() {
    return ("Syntax: brief [on | off]\n\n"
            "This command allows you to toggle brief room descriptions "
            "on or off.\nSee also: env");
}

