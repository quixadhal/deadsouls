/*    /lib/comp/seal.c
 *    from the Dead Souls V Object Library
 *    Composite component of a closeable and lockable thing
 *    Created by Descartes of Borg 961221
 *    Version: @(#) seal.c 1.2@(#)
 *    Last modified: 96/12/23
 */

#include <lib.h>

inherit LIB_CLOSE;
inherit LIB_LOCK;

varargs static mixed AddSave(mixed args...) {
    return ({});
}

mixed CanLock(object who, string id) {
    mixed tmp = lock::CanLock(who);

    if( tmp != 1 ) {
	return tmp;
    }
    if( !GetClosed() ) {
	return "You cannot lock it while it is open.";
    }
    return 1;
}

mixed CanOpen(object who) {
    if( GetLocked() ) {
	return "It is locked!";
    }
    else return close::CanOpen(who);
}

varargs mixed eventOpen(object who, object tool) {
    if( tool && GetLocked() ) {
	mixed tmp =  eventPick(who, tool);

	if( tmp != 1 || GetLocked() ) {
	    return tmp;
	}
    }
    if( GetLocked() ) {
	send_messages(({ "attempt", "find" }), "$agent_name $agent_verb to "
	              "open $target_name, but $agent_nominative $agent_verb "
	              "it locked.", who, this_object(), environment(who));
	return 1;
    }
    return close::eventOpen(who, tool);
}

static private void create() {
    AddSave(close::GetSave() + lock::GetSave());
}
