#include <lib.h>
#include <daemons.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("deactivate");
    SetRules("", "OBJ","OBJ on OBJ","STR on OBJ");
    SetErrorMessage("Deactivate something?");
    SetHelp("Syntax: deactivate THING\n"
            "        \n"
            "See also: activate");  
}

mixed can_deactivate() {
    return this_player()->CanManipulate();
}

mixed can_deactivate_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_deactivate_obj_on_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_deactivate_str_on_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed do_deactivate() {
    return "Deactivate what?";
    return 1;
}

varargs mixed do_deactivate_obj(mixed foo, mixed foo2) {
    return foo->eventTurnOff();
}

varargs mixed do_deactivate_obj_on_obj(mixed foo, mixed foo2) {
    return foo->eventTurnOff(foo2);
}

varargs mixed do_deactivate_str_on_obj(mixed foo, mixed foo2) {
    return foo2->eventTurnOff(foo);
}

