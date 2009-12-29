/*    /secure/cmds/admins/removerace.c
 *    from the NIghtmare V Object Library
 *    removes races
 *    created by Descartes of Borg 960528
 *    Version: %I%
 *    Last Modified: %D%
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    if( !args || args == "" ) return "Syntax: removerace <RACE>";
    RACES_D->RemoveRace(args);
    previous_object()->eventPrint("Race removed.");
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: removerace <RACE>\n\n"
            "Removes a race from the game.\n"
            "See also: addclass, addemote, addrace, removeclass");
}
