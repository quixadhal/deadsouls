/*    /cmds/creators/codeblock.c
 *    From the Dead Souls 1.1 Mudlib
 *    Allows a creator to block all channels
 *    Created by Descartes of Borg 921031
 *    Version: @(#) codeblock.c 1.2@(#)
 *    Last modified: 96/10/08
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    previous_object()->SetBlocked("all");
    if( previous_object()->GetBlocked("all") ) {
	previous_object()->eventPrint("All channels are being blocked.",
				      MSG_SYSTEM);
    }
    else {
	previous_object()->eventPrint("Codeblock is now off.", MSG_SYSTEM);
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: <codeblock>\n\n"
	    "Allows you to toggle on and off the blocking of all mud "
	    "channels.\n\n"
	    "See also: classblock");
}
