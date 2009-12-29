#include <lib.h>
#include <daemons.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("activate");
    SetRules("OBJ","OBJ on OBJ","STR on OBJ");
    SetErrorMessage("Activate something?");
    SetHelp("Syntax: activate THING\n"
            "        \n"
            "See also: deactivate");  
}

mixed can_activate() {
    return this_player()->CanManipulate();
}

mixed can_activate_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_activate_obj_on_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_activate_str_on_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed do_activate() {
    return "Activate what?";
    return 1;
}

varargs mixed do_activate_obj(mixed foo) {
    return foo->eventTurnOn();
}

varargs mixed do_activate_obj_on_obj(mixed foo, mixed foo2) {
    return foo->eventTurnOn(foo2);
}

varargs mixed do_activate_str_on_obj(mixed foo, mixed foo2) {
    return foo2->eventTurnOn(foo);
}

