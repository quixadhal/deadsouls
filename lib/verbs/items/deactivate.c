#include <lib.h>
#include <daemons.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("deactivate");
    SetRules("", "OBJ","OBJ on OBJ");
    SetErrorMessage("Deactivate something?");
    SetHelp("Syntax: <deactivate> THING\n"
            "        \n"
            "");  
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
