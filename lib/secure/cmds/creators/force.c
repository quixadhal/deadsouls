/*    /secure/cmds/creators/force.c
 *    From the Dead Souls V Object Library
 *    the same as call foo->eventForce("whatever")
 *    Created by Descartes of Borg 961117
 *    Version: @(#) force.c 1.3@(#)
 *    Last modified: 96/11/17
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string cmd = args;
    object who;
    int i;
	
    if( !cmd || cmd == "" ) {
	return "Syntax: <force LIVING COMMAND>";
    }
    args = "";
    while( (i = strsrch(cmd, " ")) != -1 ) {
	args = args + cmd[0..(i-1)];
	if( i == strlen(cmd) - 1 ) {
	    cmd = "";
	}
	else {
	    cmd = cmd[(i+1)..];
	}
	if( who = find_living(args) ) {
	    break;
	}
    }
    if( !who ) {
	return "Who are you looking for?";
    }
    if( archp(who) ) {
	return "You cannot do that!";
    }
    previous_object()->eventPrint("You force " + who->GetName() +
				  " to " + cmd + ".");
    if( !archp(previous_object()) ) {
	who->eventPrint("WARNING: " + previous_object()->GetCapName() +
			" forces you to: " + cmd);
    }
    who->eventForce(cmd);
    return 1;
}

string GetHelp(string topic) {
    return ("Syntax: <force LIVING COMMAND>\n\n"
	    "Allows you to force some living being to do something.  "
	    "Abuse of this command is grounds for removal from "
	    "%^I%^" + mud_name() + "%^/I%^.  Creators should only ever "
	    "use this command on test characters and NPC's in unopen areas.");
}
