/*    /lib/events/dig_with.c
 *    From the Dead Soulsr2 Object Library
 *    handles functions to allow something to dig
 *    Created by Descartes of Borg 951030
 *    Version: @(#) dig_with.c 1.3@(#)
 *    Last modified: 97/01/01
 */

#include <function.h>

varargs mixed CanBuryWith(object who, object what) {
    int fp = functionp(environment(who)->GetBury());

    if( !fp || (fp & FP_OWNER_DESTED) ) {
	return "This is not a very good place for burying.";
    }
    return 1;
}

varargs mixed CanDigWith(object who, object what) {
    if( what ) { // the parser will trigger CanDig() there
	return 1;
    }
    else {
	int fp = functionp(environment(who)->GetDig());

	if( !fp || (fp & FP_OWNER_DESTED) ) {
	    return "This is not a very good place for that kind of digging.";
	}
	return 1;
    }
}

mixed eventBuryWith(object who, object what) {
    mixed tmp = environment(who)->eventBuryItem(who, this_object(), what);

    if( tmp == 1 ) {
	return tmp;
    }
    else {
	who->eventPrint(tmp || "The burial did not work well.");
    }
    return 1;
}

varargs mixed eventDigWith(object who, object what) {
    object target;
    mixed tmp;

    if( what ) {
	target = what;
    }
    else {
	target = environment(who);
    }
    tmp = target->eventDig(who, this_object());
    if( tmp == 1 ) {
	return tmp;
    }
    else {
	who->eventPrint(tmp || "The digging did not work so well.");
	return 1;
    }
}

mixed direct_bury_str_with_obj(string str) {
    if( environment() != this_player() ) {
	return "#You must have it to dig with it!";
    }
    return CanBuryWith(this_player());
}

mixed direct_dig_with_obj() {
    if( environment() != this_player() ) {
	return "#You must have it to dig with it!";
    }
    return CanDigWith(this_player());
}

mixed direct_dig_str_with_obj(string what) {
    if( environment() != this_player() ) {
	return "#You must have it to dig with it!";
    }
    if( remove_article(lower_case(what)) != "hole" ) {
	return "Dig a what?";
    }
    return CanDigWith(this_player());
}

mixed indirect_bury_obj_with_obj(object what) {
    if( environment() != this_player() ) {
	return "#You must have it to bury with it!";
    }
    return CanBuryWith(this_player(), what);
}

mixed indirect_dig_obj_with_obj(object what) { 
    if( environment() != this_player() ) {
	return "#You must have it to dig with it!";
    }
    return CanDigWith(this_player(), what);
}
