/*    /cmds/creators/force.c
 *    From the Dead Souls V Object Library
 *    Created by Descartes of Borg 961018
 *    Version: %A%
 *    Last modified: %D%
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object who = previous_object();
    object target;
    string name, cmd;

    if( sscanf(args, "%s to %s", name, cmd) != 1 ) {
	int i = strsrch(args, " ");

	name = "";
	while( i != -1 ) {
	    name += args[0..(i-1)];
	    if( strlen(args) < i-1 ) {
		args = args[(i+1)..];
	    }
	    else {
		args = "";
	    }
	    if( target = find_living(lower_case(name)) ) {
		break;
	    }
	    if( target = find_living(convert_string(name)) ) {
		break;
	    }
	}
	if( !target ) {
	    who->eventPrint("Cannot find any living thing called: " + name);
	    return 1;
	}
	cmd = args;
    }
    else {
	target = find_living(lower_case(name));
	if( !target ) {
	    target = find_living(convert_string(name));
	}
	if( !target ) {
	    who->eventPrint("Cannot find any living thing called: " + name);
	    return 1;
	}
    }
    target->eventPrint(who->GetName() + " forces you to: " + cmd);
    who->eventPrint("You force " + target->GetShort() + " to: " + cmd);
    target->eventForce(cmd);
    return 1;
}

string GetErorMessage() {
    "Force whom to do what?";
}

string GetHelp() {
    return ("Syntax: <force LIVING to COMMAND>\n\n"
	    "Allows you to force a living object to take a certain action.");
}
