

#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("empty");
    SetRules("OBJ");
    SetErrorMessage("Empty what?");
    SetHelp("Syntax: empty <vessel>\n\n"
            "Removes the fluid from a vessel.");
}

mixed can_empty_obj(string verb) { return this_player()->CanManipulate(); }

mixed do_empty_obj(object from) {
    return from->eventEmpty(this_player());
}

