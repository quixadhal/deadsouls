#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("fill");
    SetRules("OBJ with OBJ","OBJ from OBJ");
    SetErrorMessage("Fill what with what?");
    SetHelp("Syntax: fill <vessel> with <substance>\n\n"
            "Allows you to fill a vessel designed for containing a substance.");
}

mixed can_fill_obj_with_obj(string verb) { return this_player()->CanManipulate(); }

mixed can_fill_obj_from_obj(string verb) { return this_player()->CanManipulate(); }

mixed do_fill_obj_with_obj(object to, object from) {
    return to->eventFill(this_player(), from);
}

mixed do_fill_obj_from_obj(object to, object from) {
    return to->eventFill(this_player(), from);
}

