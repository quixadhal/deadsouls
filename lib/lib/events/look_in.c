/*    /lib/events/look_in.c
 *    From the Dead Souls V Object Library
 *    Something that can be seen in
 *    Created by Descartes of Borg 961222
 *    Version :@(#) look_in.c 1.2@(#)
 *    Last modified: 96/12/31
 */

#include <function.h>

private string InternalDesc = 0;
private int    Opacity = 100;

// abstract methods
string GetShort();
// end abstract methods

string GetInternalDesc() {
    mixed val = InternalDesc;

    if( arrayp(val) ) {
	val = val[query_night()];
    }
    if( stringp(val) ) {
	return val;
    }
    else if( functionp(val) ) {
	if( functionp(val) & FP_OWNER_DESTED ) {
	    return "Error evaluating internal description.";
	}
	return evaluate(val, this_player());
    }
}

string SetInternalDesc(string str) {
    return (InternalDesc = str);
}

int GetOpacity() {
    return Opacity;
}

static int SetOpacity(int x) {
    Opacity = x;
    parse_refresh();
    return Opacity;
}

varargs mixed CanShowInterior(object who, object target) {
    int x;
    
    if( target ) {
	x = 66;
    }
    else {
	x = 34;
    }
    if( GetOpacity() > x ) {
	return 0;
    }
    return 1;
}

mixed eventShowInterior(object who, object target) {
    if( target ) {
	return target->eventShow(who);
    }
    else {
	string str = GetInternalDesc();

	if( !str || str == "" ) {
	    return 0;
	}
	environment(who)->eventPrint(who->GetName() + " looks inside " +
				     GetShort() + ".", who);
	who->eventPrint(str);
    }
}

mixed direct_look_in_obj() {
    return CanShowInterior(this_player());
}

mixed direct_look_inside_obj() {
    return CanShowInterior(this_player());
}

mixed indirect_look_at_obj_word_obj(object target) {
    if( environment(target) != this_object() ) {
	return "#That is not in there.";
    }
    return CanShowInterior(this_player(), target);
}

mixed inventory_visible() {
    if( GetOpacity() > 33 ) {
	return 0;
    }
    else {
	return 1;
    }
}
