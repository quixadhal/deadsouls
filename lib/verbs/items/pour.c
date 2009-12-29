

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("pour");
    SetRules("from OBJ into OBJ","from OBJ in OBJ", "OBJ in OBJ","OBJ into OBJ","OBJ on OBJ","OBJ onto OBJ","OBJ out","out OBJ");
    SetErrorMessage("Pour what into what?");
    SetHelp("Syntax: pour <substance> into <vessel>\n\n"
            "Allows you to pour into a vessel designed for containing a substance.");
}

mixed can_pour_obj_into_obj(string verb) { return this_player()->CanManipulate(); }

mixed can_pour_obj_in_obj(string verb) { return this_player()->CanManipulate(); }

mixed can_pour_obj_onto_obj(string verb) { return this_player()->CanManipulate(); }

mixed can_pour_obj_on_obj(string verb) { return this_player()->CanManipulate(); }

mixed can_pour_obj_out(string verb) { return this_player()->CanManipulate(); }

mixed can_pour_out_obj(string verb) { return this_player()->CanManipulate(); }

mixed can_pour_from_obj_into_obj(string verb) { return this_player()->CanManipulate(); }

mixed do_pour_from_obj_into_obj(object from, object to) {
    return to->eventPour(this_player(), from);
}

mixed do_pour_from_obj_in_obj(object from, object to) {
    return to->eventPour(this_player(), from);
}

mixed do_pour_obj_onto_obj(object from, object to) {
    return to->eventPour(this_player(), from);
}

mixed do_pour_obj_on_obj(object from, object to) {
    return do_pour_obj_onto_obj(from, to);
}

mixed do_pour_obj_in_obj(object from, object to) {
    return do_pour_obj_onto_obj(from, to);
}

mixed do_pour_obj_into_obj(object from, object to) {
    return do_pour_obj_onto_obj(from, to);
}

mixed do_pour_obj_out(object from) {
    return from->eventEmpty(this_player());
}

mixed do_pour_out_obj(object from) {
    return from->eventEmpty(this_player());
}



