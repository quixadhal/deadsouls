/*    /secure/cmds/admins/removeadverb.c
 *    From the Dead Souls V Object Library
 *    Command for removing adverbs from the soul
 *    Created by Descartes of Borg 961214
 *    Version: @(#) removeadverb.c 1.1@(#)
 *    Last modified: 96/12/14
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    if( !args || args == "" ) {
	return "Remove which adverb?";
    }
    if( !SOUL_D->RemoveAdverb(args) ) {
	previous_object()->eventPrint("Removal of adverb failed.");
	return 1;
    }
    previous_object()->eventPrint("Adverb removed.");
    return 1;
}

string GetHelp(string args) {
    return ("Syntax: <removeadverb ADVERB>\n\n"
	    "Allows you to remove an adverb from the list of"
	    "system adverbs that can be used as adverbs in most soul "
	    "commands.\n\n"
	    "See also: addadverb, addemote, removeemote");
}
