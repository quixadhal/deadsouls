/*    /secure/cmds/admins/removeemote.c
 *    From the Dead Souls V Object Library
 *    Command for removing commands from the soul
 *    Created by Descartes of Borg 961214
 *    Version: @(#) removeemote.c 1.2@(#)
 *    Last modified: 96/12/14
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string verb, rule;
    
    if( !args || args == "" ) {
	return "Remove which emote?";
    }
    if( sscanf(args, "%s %s", verb, rule) != 2 ) {
	verb = args;
	rule = 0;
    }
    if( !rule ) {
	if( !SOUL_D->RemoveVerb(verb) ) {
	    previous_object()->eventPrint("Removal of verb failed.");
	    return 1;
	}
	previous_object()->eventPrint("Verb removed.");
    }
    else {
	if( !SOUL_D->RemoveRule(verb, rule) ) {
	    previous_object()->eventPrint("Removal of rule failed.");
	    return 1;
	}
	previous_object()->eventPrint("Rule removed.");
    }
    return 1;
}

string GetHelp(string args) {
    return ("Syntax: <removeemote EMOTE (RULE)>\n\n"
	    "Allows you to remove an emote or a rule for an emote from "
	    "the soul.  If you specify a rule, only that rule gets removed.  "
	    "If you fail to specify a rule, the entire emote is removed.\n\n"
	    "See also: addadverb, addemote, removeadverb");
}
