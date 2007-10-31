/*    /verbs/rooms/fly.c
 *    from the Dead Souls Object Library
 *    Like go, except for flying around
 *    created by Descartes of Borg 961013
 *    Version: @(#) fly.c 1.2@(#)
 *    Last modified: 96/10/13
 */

#include <lib.h>
#include <medium.h>
#include <daemons.h>
#include <position.h>
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
        return "You are nowhere to begin with!";
    }

    if(this_player()->GetPosition() == POSITION_FLYING) 
        return "You are already flying.";

    switch( env->GetMedium() ) {
    case MEDIUM_LAND:
        if(env->CanFly(this_player())) return this_player()->CanFly();
    case MEDIUM_AIR:
        return this_player()->CanFly();
    default:
        return "You can't fly here.";
    }
}

mixed can_fly_str(string str) {
    object env = environment(this_player());
    int envpos = env->GetPosition();
    //tc("1");
    if( !env ) {
        //tc("2");
        return "You are nowhere.";
    }
    if( (int)this_player()->GetStaminaPoints() < 15 ){
        //tc("3");
        return "You are too tired to fly anywhere right now.";
    }
    //tc(identify(env)+"->CanFly("+identify(this_player())+", "+str+"): "+identify(env->CanFly(this_player(), str)));
    if(env->CanFly(this_player(), str)){
        //tc("42");
        if(envpos == POSITION_FLYING) return 1;
        return this_player()->CanFly();
    }
    if(this_player()->GetPosition() != POSITION_FLYING &&
      envpos != POSITION_FLYING){
        return "You are not flying.";
    }
    //tc("999");
    return 0;
}

mixed can_fly_into_str(string str) {
    object env = environment(this_player());
    int envpos = env->GetPosition();
    if( !env ) {
        return "You are nowhere.";
    }
    if( (int)this_player()->GetStaminaPoints() < 3 )
        return "You are too tired right now.";
    if((mixed)environment(this_player())->CanEnter(this_player(), str)){
        //tc("23");
        if(envpos == POSITION_FLYING) return 1;
        return this_player()->CanFly();
    }
    if(this_player()->GetPosition() != POSITION_FLYING &&
      envpos != POSITION_FLYING){
        return "You are not flying.";
    }

    else return 0;
}

mixed do_fly() {
    return this_player()->eventFly();
}

mixed do_fly_str(string str) {
    this_player()->AddStaminaPoints(-1);
    return (mixed)environment(this_player())->eventGo(this_player(), str);
}

mixed do_fly_into_str(string str) {
    this_player()->AddStaminaPoints(-1);
    return (mixed)environment(this_player())->eventEnter(this_player(), str);
}
