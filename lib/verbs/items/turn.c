/*    /verbs/items/turn.c
 *    from the Frontiers Object Library
 *    turn STR
 *    turn on ITEM
 *    etc
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("turn");
    SetRules("", "OBJ", "on OBJ", "off OBJ", "OBJ on", "OBJ off");
    SetErrorMessage("Turn on or turn off something?");
    SetHelp("Syntax: <turn>\n"
            "        <turn on ITEM>\n"
            "        <turn off ITEM>\n"
            "        <turn on THING on ITEM>\n"
            "        <turn off THING on ITEM>\n\n"
            "");  
}

mixed can_turn() {
    return this_player()->CanManipulate();
}

mixed can_turn_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_turn_on_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_turn_off_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_turn_obj_on(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_turn_obj_off(mixed foo) {
    return this_player()->CanManipulate();
}

mixed do_turn() {
    environment(this_player())->eventPrint(this_player()->GetName() +
            " turns around.", this_player());
    this_player()->eventPrint("You turn around.");
    return 1;
}

varargs mixed do_turn_obj(mixed foo, mixed foo2) {
    return foo->eventTurn();
}

varargs mixed do_turn_on_obj(mixed foo, mixed foo2) {
    return foo->eventTurnOn();
}

mixed do_turn_off_obj(mixed foo) {
    return foo->eventTurnOff();
}

varargs mixed do_turn_obj_on(mixed foo, mixed foo2) {
    return foo->eventTurnOn();
}

mixed do_turn_obj_off(mixed foo) {
    return foo->eventTurnOff();
}

mixed do_turn_on_obj_word_obj(mixed foo, mixed foo2) {
    return foo2->eventTurnOn(foo);
}
