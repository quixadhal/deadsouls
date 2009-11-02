#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("disarm");
    SetRules("OBJ");
    SetErrorMessage("Disarm what?");
    SetHelp("Syntax: <disarm OBJ with OBJ>\n\n"
            "Use this command to try to disarm a boobytrapped object, like "
            "a door or a chest.\n\n"
            "See also: close, open, pick, disarm");
}

varargs mixed can_disarm_obj(mixed args...) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

varargs mixed do_disarm_obj(object target) {
    return target->eventDisarm(this_player());
}
