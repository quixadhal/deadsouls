/*    /secure/cmds/creator/more.c
 *    from the Dead Soulsr2 Object Library
 *    more FILE
 *    created by Descartes of Borg 951104
 */

#include <lib.h>
#include <message_class.h>
#include "include/more.h"

inherit LIB_DAEMON;

mixed cmd(string str) {
    if( !str || str == "" ) return "Syntax: <more FILE>";
    return (mixed)this_player()->eventPage(str, MSG_SYSTEM | MSG_NOCOLOUR);
}

string GetHelp(string str) {
    return ("Syntax: <more FILE>\n\n"
	    "Pages through the file you name.  While inside the pager, you "
	    "have access to the following commands:\n" +
	    (string)LIB_PAGER->GetHelp("pager") + "\n\n"
	    "See also: cat, ed, head, tail");
}
