/*    /verbs/players/sit.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960711
 *    Version: @(#) sit.c 1.7@(#)
 *    Last Modified: 96/12/21
 */

#include <lib.h>
#include <position.h>
#include <rounds.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("sit");
    SetRules("", "up","down", "down in OBJ", "down on OBJ", "in OBJ", "on OBJ");
    SetErrorMessage("Sit down?");
    SetHelp("Syntax: sit [down]\n"
            "        sit down in <OBJ>\n\n"
            "Allows you to sit down on the ground or in a chair-like object."
            "\nSee also: lie, stand");
}

mixed can_sit_down() {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything!";
    }
    if(!environment(this_player())->CanSit(this_player())){
        return "You can't sit here.";
    }
    if( this_player()->GetPosition() == POSITION_SITTING ) {
        return "You are already seated!";
    }
    if( this_player()->GetPosition() != POSITION_STANDING ) {
        return "You must be standing in order to sit down!";
    }
    return 1;
}

mixed can_sit_up() {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything!";
    }
    if( this_player()->GetPosition() != POSITION_LYING ) {
        return "You must be lying in order to sit up!";
    }
    return 1;
}

mixed can_sit(){
    if(this_player()->GetPosition() == POSITION_LYING) return can_sit_up();
    else return can_sit_down();
}

mixed can_sit_word_obj() {
    return can_sit_down();
}

mixed can_sit_down_word_obj() {
    return can_sit_down();
}

mixed do_sit(){
    return this_player()->eventSit();
}

mixed do_sit_down() {
    return this_player()->eventSit();
}

mixed do_sit_up() {
    return this_player()->eventSit();
}


mixed do_sit_word_obj(string word, object target) {
    return this_player()->eventSit(target);
}

mixed do_sit_down_word_obj(string word, object target) {
    return this_player()->eventSit(target);
}
