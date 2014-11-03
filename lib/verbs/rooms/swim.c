#include <lib.h>
#include <medium.h>
#include <daemons.h>
#include <position.h>
#include "include/swim.h"

inherit LIB_VERB;

static void create(){
    verb::create();
    SetVerb("swim");
    SetRules("", "STR", "into STR");
    SetErrorMessage("Swim in which direction?");
    SetHelp("Syntax: swim <DIRECTION>\n"
            "        swim into <PLACE>\n\n"
            "Moves you towards the direction you specify, or into the place "
            "you specify.  The command \"swim into\" is synonymous with the "
            "\"enter\" command.\n"  
            "See also: fly, climb, enter, go, jump");
}

int StaminaCost(){
    int cost = 20;
    int bonus = this_player()->GetStatLevel("durability")/10;
    bonus += this_player()->GetStatLevel("strength")/20;
    bonus += this_player()->GetFood()/40;
    bonus += this_player()->GetDrink()/40;
    cost -= bonus;
    if(cost < 1) cost = 1;
    return cost;
}

mixed can_swim(){
    object env = environment(this_player());
    if( !env ){
        return "You are nowhere to begin with!";
    }
    if(this_player()->GetPosition() == POSITION_SWIMMING) 
        return "You are already swimming.";
    if(!RACES_D->CanSwim(this_player()->GetRace())){
        return capitalize(pluralize(this_player()->GetRace()))+" cannot swim.";
    }
    if(env->CanSwim(this_player())) return this_player()->CanSwim();
    return "You can't swim here.";
}

mixed can_swim_str(string str){
    object env = environment(this_player());
    int envpos = env->GetPosition();
    if( !env ){
        return "You are nowhere.";
    }
    if( this_player()->GetStaminaPoints() < 15 ){
        return "You are too tired to swim anywhere right now.";
    }
    if(env->CanSwim(this_player(), str)){
        if(envpos == POSITION_SWIMMING) return 1;
        return this_player()->CanSwim();
    }
    if(this_player()->GetPosition() != POSITION_SWIMMING &&
            envpos != POSITION_SWIMMING){
        return "You are not swimming.";
    }
    return 0;
}

mixed can_swim_into_str(string str){
    object env = environment(this_player());
    int envpos = env->GetPosition();
    if( !env ){
        return "You are nowhere.";
    }
    if( this_player()->GetStaminaPoints() < 3 )
        return "You are too tired right now.";
    if(environment(this_player())->CanEnter(this_player(), str)){
        if(envpos == POSITION_SWIMMING) return 1;
        return this_player()->CanSwim();
    }
    if(this_player()->GetPosition() != POSITION_SWIMMING &&
            envpos != POSITION_SWIMMING){
        return "You are not swimming.";
    }
    return 0;
}

mixed do_swim(){
    return this_player()->eventSwim();
}

mixed do_swim_str(string str){
    this_player()->AddStaminaPoints(-StaminaCost());
    return environment(this_player())->eventGo(this_player(), str);
}

mixed do_swim_into_str(string str){
    this_player()->AddStaminaPoints(-StaminaCost());
    return environment(this_player())->eventEnter(this_player(), str);
}
