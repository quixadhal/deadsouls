/*    /secure/cmds/admins/removeclass.c
 *    from the NIghtmare V Object Library
 *    removes classes
 *    created by Descartes of Borg 960528
 *    Version: %I%
 *    Last Modified: %D%
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    if( !args || args == "" ) return "Syntax: <removeclass CLASS>";
    CLASSES_D->RemoveClass(args);
    previous_object()->eventPrint("Class removed.");
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <removeclass CLASS>\n\n"
	    "Removes a class from the game.\n\n"
	    "See also: addclass, addemote, addrace, removerace");
}
