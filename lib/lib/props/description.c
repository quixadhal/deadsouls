/*    /lib/props/description.c
 *    From the Dead Souls V Object Library
 *    Handles the describing of objects
 *    Created by Descartes of Borg 961222
 *    Version: @(#) description.c 1.1@(#)
 *    Last modified: 96/12/22
 */

#include <function.h>

private int   ProperNoun = 0;
private mixed Short      = 0;

string GetShort();

string GetDefiniteShort() {
    string tmp = GetShort();

    if( !tmp ) {
	return 0;
    }
    if( ProperNoun ) {
	return tmp;
    }
    else {
	return add_article(tmp, 1);
    }
}
    
string GetShort() {
    if( !Short ) {
	return 0;
    }
    if( functionp(Short) ) {
	if( functionp(Short) & FP_OWNER_DESTED ) {
	    return "Error in evaluating function pointer.";
	}
	return evaluate(Short);
    }
    else if( arrayp(Short) ) {
	return Short[query_night()];
    }
    else {
	return Short;
    }
}

varargs mixed SetShort(mixed val, int proper) {
    if( !stringp(val) && !functionp(val) && !arrayp(val) ) {
	error("Bad argument 1 to SetShort().\n");
    }
    if( proper ) {
	ProperNoun = proper;
    }
    else if( stringp(val) ) { // add a support for most old stuff...
	if( strlen(val) > 2 ) {
	    string tmp = lower_case(val[0..2]);

	    if( tmp[0..1] != "a " && tmp != "an " ) {
		ProperNoun = 1;
	    }
	}
	else {
	    ProperNoun = 1;
	}
    }
    return (Short = val);
}

