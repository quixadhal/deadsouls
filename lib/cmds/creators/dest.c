/*    /cmds/creator/dest.c
 *    From the Dead Souls V Object Library
 *    Destructs the object named
 *    Created by Descartes of Borg 950425
 *    Version: @(#) dest.c 1.2@(#)
 *    Last modified: 96/10/08
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object who = previous_object();
    object target;
    string msg;
    
    if( !args || args == "" ) {
	return "Dest what?";
    }
    if( args == "all" ) {
	foreach(target in all_inventory(environment(who))) {
	    if( interactive(target) ) {
		continue;
	    }
	    msg = who->GetMessage("dest", target);
	    who->eventPrint("You dest " + target->GetShort() + ".",
			    MSG_SYSTEM);
	    environment(who)->eventPrint(msg, MSG_SYSTEM, previous_object());
	    target->eventDestruct();
	    if( target ) {
		destruct(target);
	    }
        }
        return 1;
    }
    if( archp(who) ) {
	target = to_object(args);
    }
    if( !target && !(target = present(lower_case(args), who))  &&
	!(target = present(lower_case(args), environment(who))) ) {
	return "No such thing.";
    }
    if( interactive(target) && !archp(who) ) {
	return "You are not permitted to do that.";
    }
    msg = who->GetMessage("dest", target);
    who->eventPrint("You dest " + target->GetShort() + ".", MSG_SYSTEM);
    if( interactive(target) ) {
	target->eventPrint(who->GetName() + " dests you.", MSG_SYSTEM);
    }
    environment(who)->eventPrint(msg, MSG_SYSTEM, ({ who, target }));
    target->eventDestruct();
    if( target ) {
	destruct(target);
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: <dest [object]>\n\n"
	    "This command destroys the object named.  \"dest all\" "
	    "destroys all the items in your inventory.  See \"help format\" "
	    "for how to denote objects.\n\n"
	    "See also: clean, scan");
}
