/*    /cmds/creators/move.c
 *    From the Dead Souls V Object Library
 *    Moves object from one place to another
 *    created by Descartes of Borg 961008
 *    Version: @(#) move.c 1.2@(#)
 *    Last modified: 96/10/08
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object what, destination;
    string a, b;
    mixed res;
    
    if( sscanf(args, "%s into %s", a, b) != 2 &&
	sscanf(args, "%s to %s", a, b) != 2 ) {
	return "Syntax: <move OBJECT INTO OBJECT>\n\n";
    }
    what = to_object(a);
    destination = to_object(b);
    if( !what ) {
	return "Unable to find " + a + ".";
    }
    if( !destination ) {
	return "Unable to find " + b + ".";
    }
    if( living(what) && living(destination) ) {
	return "None of that nonsense.";
    }
    res = what->eventMove(destination);
    if( !res ) {
	return "Failed to move " + identify(what) + " into " +
	    identify(destination) + ".";
    }
    else if( res != 1 ) {
	return res;
    }
    previous_object()->eventPrint("Moved " + identify(what) + " into " +
				  identify(destination) + ".", MSG_SYSTEM);
    if( living(what) ) {
	what->eventDescribeEnvironment();
    }
    return 1;
}

string GetHelp() {
    return ("Syntax: <move OBJECT into OBJECT>\n\n"
	    "Allows you to move the object you name into the container "
	    "you name.\n\n"
	    "See also: trans");
}
