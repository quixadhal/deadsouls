/*    /verbs/rooms/fly.c
 *    from the Dead Souls V Object Library
 *    Like go, except for flying around
 *    created by Descartes of Borg 961013
 *    Version: @(#) fly.c 1.2@(#)
 *    Last modified: 96/10/13
 */

#include <lib.h>
#include <medium.h>
#include "include/fly.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("fly");
    SetRules("", "STR", "into STR");
    SetErrorMessage("Fly in which direction?");
    SetHelp("Syntax: <fly DIRECTION>\n"
	    "        <fly into PLACE>\n\n"
	    "Moves you towards the direction you specify, or into the place "
	    "you specify.  The command \"fly into\" is synonymous with the "
	    "\"enter\" command.\n\n"  
	    "See also: climb, enter, go, jump");
}

mixed can_fly() {
    object env = environment(this_player());

    if( !env ) {
	return "You are nowhere tobegin with!";
    }
    switch( env->GetMedium() ) {
    case MEDIUM_LAND:
	return env->CanFly(this_player());
    case MEDIUM_AIR:
	return "Fly where?";
    default:
	return "You can't fly here.";
    }
}
    
mixed can_fly_str(string str) {
    if( !environment(this_player()) ) {
	return "You are nowhere.";
    }
    if( !creatorp(this_player()) && (int)this_player()->GetStaminaPoints() < 15 )
      return "You are too tired to fly anywhere right now.";
    return (mixed)environment(this_player())->CanFly(this_player(), str);
}

mixed can_fly_into_str(string str) {
    if( !environment(this_player()) ) {
	return "You are nowhere.";
    }
    if( !creatorp(this_player()) && (int)this_player()->GetStaminaPoints() < 3 )
	return "You are too tired right now.";
    return (mixed)environment(this_player())->CanEnter(this_player(), str);
}

mixed do_fly() {
    return environment(this_player())->eventFly(this_player());
}

mixed do_fly_str(string str) {
    return (mixed)environment(this_player())->eventFly(this_player(), str);
}

mixed do_fly_into_str(string str) {
    return (mixed)environment(this_player())->eventEnter(this_player(), str);
}
