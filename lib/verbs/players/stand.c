/*    /verbs/players/stand.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960711
 *    Version: @(#) stand.c 1.5@(#)
 *    Last Modified: 96/12/21
 */

#include <lib.h>
#include <daemons.h>
#include <position.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("stand");
    SetRules("", "up");
    SetErrorMessage("Stand up?");
    SetSynonyms("get up");
    SetHelp("Syntax: stand [up]\n"
            "When sitting down or lying down, you can get up in this "
            "most intuitive manner.\n"
            "See also: lie, sit");
}

mixed can_stand_up() {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    if(RACES_D->GetLimblessRace(this_player()->GetRace()) ){
        return "You aren't endowed with limbs with which to stand.";
    }

    if(!environment(this_player())->CanStand(this_player())){ 
        return "You can't stand here.";
    }

    if( this_player()->GetPosition() != POSITION_STANDING ) {
        return 1;
    }
    return "You are already standing up!";
}

mixed can_stand(){
    return can_stand_up();
}

mixed do_stand_up() {
    return this_player()->eventStand();
}

mixed do_stand() {
    return this_player()->eventStand();
}
