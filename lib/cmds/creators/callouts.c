/*    /cmds/creator/callouts.c
 *    from the Dead Souls LPC Library
 *    lists all callouts pending
 *    created by Descartes of Borg 950514
 *    Version: @(#) callouts.c 1.3@(#)
 *    Last modified: 96/10/06
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    mixed array callouts;
    string tmp;
    
    callouts = call_out_info();
    if( !sizeof(callouts) ) {
	previous_object()->eventPrint("No pending callouts.", MSG_SYSTEM);
	return 1;
    }
    tmp = sprintf("%:-40s %:-25s Delay\n", "Object", "Function");
    tmp += "--------------------------------------------------"
      "-------------------------\n";
    foreach(mixed array callout in callouts) {
	tmp += sprintf("%:-40O %:-25s %d\n", callout...);
    }
    this_player()->eventPage(explode(tmp, "\n"), MSG_SYSTEM);
    return 1;
}

string GetHelp() {
    return ("Syntax: <callouts>\n\n"
	    "Lists all pending callouts.\n\n"
	    "See also: events, mstatus, netstat");
}
