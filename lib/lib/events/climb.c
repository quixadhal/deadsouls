/*    /lib/climb.c
 *    from the Dead Souls V Object Library
 *    handles different methods of climbing
 *    created by Descartes of Borg 960120
 *    Version: @(#) climb.c 1.7@(#)
 *    Last modified: 96/12/22
 */

#include <climb.h>
#include <function.h>

static private mapping Climb = 0;

// abstract methods
string GetDefiniteShort();
// end abstract methods

mapping GetClimbs() {
    return Climb;
}

varargs static mapping SetClimb(mixed val, int type) {
    if( !type ) {
	type = CLIMB_UP;
    }
    if( !Climb ) {
	Climb = ([ type : val ]);
    }
    else {
	Climb[type] = val;
    }
    return Climb;
}

mixed CanClimb(object who, int type) {
    int array tmp;
    
    if( Climb[type] ) {
	return 1;
    }
    tmp = keys(Climb);
    if( !sizeof(tmp) ) {
	return 0;
    }
    else {
	type = tmp[0];
    }
    switch(type) {
	case CLIMB_UP:
	return "Perhaps you mean to climb up it?";

	case CLIMB_DOWN:
	return "Perhaps you mean to climb down it?";

	case CLIMB_OUT:
	return "Perhaps you mean to climb out of it?";

	case CLIMB_INTO:
	return "Perhaps you mean to climb into it?";

	case CLIMB_THROUGH:
	return "Perhaps you mean to climb through it?";
    }
    return 0;
}

mixed eventClimb(object who, int type) {
    mixed dest = Climb[type];

    if( functionp(dest) ) {
	if( functionp(dest) & FP_OWNER_DESTED ) {
	    who->eventPrint("There will be no climbing that today!");
	    return 1;
	}
	return evaluate(dest, who, type);
    }
    else {
	string omsg, imsg;

	switch(type) {
	    case CLIMB_UP:
	    omsg = "$N climbs up " + GetDefiniteShort() + ".";
	    imsg = "$N comes climbing in.";
	    break;

	    case CLIMB_DOWN:
	    omsg = "$N climbs down " + GetDefiniteShort() + ".";
	    imsg = "$N comes climbing in.";
	    break;

	    case CLIMB_OUT:
	    omsg = "$N climbs out " + GetDefiniteShort() + ".";
	    imsg = "$N comes climbing in.";
	    break;

	    case CLIMB_INTO:
	    omsg = "$N climbs into " + GetDefiniteShort() + ".";
	    imsg = "$N comes climbing in.";
	    break;

	    case CLIMB_THROUGH:
	    omsg = "$N climbs through " + GetDefiniteShort() + ".";
	    imsg = "$N comes climbing in.";
	    break;
	}
	who->eventMoveLiving(dest, omsg, imsg);
	return 1;
    }
}

mixed direct_climb_obj(object ob) {
   return CanClimb(this_player(), CLIMB_UP);
}

mixed direct_climb_out_of_obj(object ob) {
    return CanClimb(this_player(), CLIMB_OUT);
}

mixed direct_climb_word_obj(string word, object ob) {
    if( !Climb ) {
	return 0;
    }
    switch(word) {
        case "up":
	return CanClimb(this_player(), CLIMB_UP);

        case "down":
	return CanClimb(this_player(), CLIMB_DOWN);

        case "out":
	return CanClimb(this_player(), CLIMB_OUT);

        case "in": case "into":
	return CanClimb(this_player(), CLIMB_INTO);

        case "through":
	return CanClimb(this_player(), CLIMB_THROUGH);

        default:
	return 0;
    }
}
