/*    /lib/jump.c
 *    from the Dead Souls V Object Library
 *    handles different methods of jumping
 *    created by Descartes of Borg 960117
 *    Version: @(#) jump.c 1.4@(#)
 *    Last modified: 96/12/22
 */

#include <jump.h>
#include <function.h>

private mapping Jumps = ([]);

mapping GetJumps() {
    return Jumps;
}

varargs mixed SetJump(mixed args) {
if(!mapp(args)) return "Bad argument to SetJump.";
foreach(mixed key, mixed val in args) {
if(!stringp(key)) return "Bad key value in SetJump";
if(!arrayp(val)) return "Bad element to SetJump key \""+key+"\". An array is required.";
if(!stringp(val[0]) && !functionp(val[0])) return "Bad element 0 in array for key \""+key+"\". A string or function is required.";
if(!intp(val[1])) return "Bad element 1 in array for key \""+key+"\". A jump type is required.";
}

return Jumps = args;

}

mixed CanJump(object who, string id, int type) {
    if( Jumps[id] && Jumps[id][1] == type ) {
	return 1;
    }
    if( !sizeof(Jumps) ) {
	return 0;
    }
    type = Jumps[keys(Jumps)[0]][1];
    switch(type) {
	case JUMP_INTO:
	return "Perhaps you mean to jump into it?";

	case JUMP_FROM:
	return "Perhaps you mean to jump from it?";

	case JUMP_THROUGH:
	return "Perhaps you mean to jump through it?";

	case JUMP_OVER:
	return "Perhaps you mean to jump over it?";

	case JUMP_ON:
	return "Perhaps you mean to jump on it?";
    }
    return 0;
}

mixed eventJump(object who, string id, int type) {
    mixed dest;
    
    if( Jumps[id] && Jumps[id][1] == type ) {
	dest = Jumps[id][0];
    }
    else {
	return 0;
    }
    if( stringp(dest) ) {
	string omsg, imsg;

	switch(type) {
	    case JUMP_INTO:
	    omsg = "$N jumps into the " + id + ".";
	    imsg = "$N comes landing in.";
	    break;

	    case JUMP_FROM:
	    omsg = "$N jumps out of here.";
	    imsg = "$N comes landing in.";
	    break;

	    case JUMP_THROUGH:
	    omsg = "$N jumps through the " + id + ".";
	    imsg = "$N comes landing in.";
	    break;

	    case JUMP_OVER:
	    omsg = "$N jumps over the " + id + ".";
	    imsg = "$N comes landing in.";
	    break;
	}
	who->eventMoveLiving(dest, omsg, imsg);
	return 1;
    }
    else {
	if( functionp(dest) & FP_OWNER_DESTED ) {
	    return "You encountered an error in a functional.";
	}
	return evaluate(dest, who, id, type);
    }
}

mixed direct_jump_word_obj(string prep, object target, string id) {
    int type;

    switch(prep) {
    case "in": case "into":
	type = JUMP_INTO;
	break;
    case "from": case "out": case "off":
	type = JUMP_FROM;
	break;
    case "through": case "across":
	type = JUMP_THROUGH;
	break;
    case "over":
	type = JUMP_OVER;
	break;
    case "on": case "onto":
	type = JUMP_ON;
	break;
    default:
	return 0;
    }
    id = remove_article(lower_case(id));
    return CanJump(this_player(), id, type);    
}

varargs mixed direct_jump_word_word_obj(mixed args...) {
    string prep = args[0] + " " + args[1];
    string id = remove_article(lower_case(args[5]));
    int type;
    
    switch(prep) {
    case "out of": case "off of":
	type = JUMP_FROM;
    default:
	return 0;
    }
    return CanJump(this_player(), id, type);
}

