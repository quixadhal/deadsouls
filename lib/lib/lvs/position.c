/*    /lib/position.c
 *    From the Dead Souls V Object Library
 *    Handles the positioning of living things
 *    Created by Descartes of Borg 961221
 *    Version: @(#) position.c 1.1@(#)
 *    Last modified: 96/12/21
 */

#include <position.h>

private int           Position = POSITION_STANDING;
private static object Chair    = 0;

// abstract methods
varargs mixed eventPrint(string str, mixed args...);
// end abstract methods

int GetPosition() {
    return Position;
}

static int SetPosition(int x) {
    Position = x;
}

mixed eventFall() {
    send_messages("fall", "$agent_name $agent_verb to the ground.",
		  this_object(), 0, environment());
    Position = POSITION_LYING;
    return 1;
}

varargs mixed eventLay(object target) {
    mixed tmp;

    if( Position != POSITION_STANDING ) {
	eventPrint("You must be standing in order to lie.");
	return 1;
    }
    if( !target ) {
	send_messages("lay", "$agent_name $agent_verb down.", this_object(),
		      0, environment());
	Position = POSITION_LYING;
	return 1;
    }
    tmp = target->eventReceiveLay(this_object());
    if( tmp != 1 ) {
	if( !tmp ) {
	    eventPrint("You cannot lay there!");
	}
	else {
	    eventPrint(tmp);
	}
	return 1;
    }
    send_messages("lay", "$agent_name $agent_verb down on " +
		  target->GetShort() + ".", this_object(), 0, environment());
    Position = POSITION_LYING;
    Chair = target;
    return 1;
    
}

varargs mixed eventSit(object target) {
    mixed tmp;

    if( Position != POSITION_STANDING ) {
	eventPrint("You must be standing in order to sit!");
	return 1;
    }
    if( !target ) {
	send_messages("sit", "$agent_name $agent_verb down.", this_object(),
		      0, environment());
	Position = POSITION_SITTING;
	return 1;
    }
    tmp = target->eventReceiveSit(this_object());
    if( tmp != 1 ) {
	if( !tmp ) {
	    eventPrint("You cannot sit there!");
	}
	else {
	    eventPrint(tmp);
	}
	return 1;
    }
    send_messages("sit", "$agent_name $agent_verb down on " +
		  target->GetShort() + ".", this_object(), 0, environment());
    Position = POSITION_SITTING;
    Chair = target;
    return 1;
}

mixed eventStand() {
    if( Position == POSITION_STANDING ) {
	eventPrint("You are already standing!");
	return 1;
    }
    if( Chair ) {
	mixed tmp = Chair->eventReleaseStand(this_object());

	if( tmp != 1 ) {
	    if( !tmp ) {
		eventPrint("You cannot get up!");
	    }
	    else {
		eventPrint(tmp);
	    }
	    return 1;
	}
	Chair = 0;
    }
    Position = POSITION_STANDING;
    send_messages("stand", "$agent_name $agent_verb up.", this_object(), 0,
		  environment());
    return 1;
}
