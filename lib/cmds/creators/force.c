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

    if(!args || args==""){
	who->eventPrint("Force whom to do what?");
	return 1;
    }
    if( sscanf(args, "%s to %s", name, cmd) < 1 ) {
	if(sscanf(args, "%s %s", name,cmd) < 1) {
	    who->eventPrint("Force whom to do what?");
	    return 1;
	}
    }
    target = present(lower_case(name),environment(who));
    if( !target ) {
	who->eventPrint("Cannot find any living thing called: " + name);
	return 1;
    }
    if(archp(target) && !archp(who)){
	who->eventPrint(target->GetName()+" shakes "+possessive(target)+
	  " head and forces you to dest yourself.");
	tell_room(environment(who), who->GetName()+" dests "+objective(who)+
	  "self while trying to pull a foolish joke on "+target->GetName()+".", who);
	who->eventDestruct();
	return 1;
    }
    target->eventPrint(who->GetName() + " forces you to: " + cmd);
    who->eventPrint("You force " + target->GetShort() + " to: " + cmd);
    target->eventForce(cmd);
    return 1;
}

string GetErorMessage() {
    return "Force whom to do what?";
}

string GetHelp() {
    return ("Syntax: <force LIVING to COMMAND>\n\n"
      "Allows you to force a living object to take a certain action.");
}
