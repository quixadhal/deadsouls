#include <lib.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("boobytrap");
    SetRules("OBJ with OBJ");
    SetErrorMessage("Boobytrap what with what?");
    SetHelp("Syntax: <boobytrap OBJ with OBJ>\n\n"
            "The first object is something you wish to boobytrap, like a "
            "door or a chest.  The second is the thing you wish to use to "
            "boobytrap it with.\n\n"
            "See also: close, open, pick, disarm");
}

mixed can_boobytrap_obj_with_obj(string verb) {
    if( this_player()->GetParalyzed() ) {
        return "You cannot do anything.";
    }
    return this_player()->CanManipulate();
}

varargs mixed do_boobytrap_obj_with_obj(object target, object trap, mixed *words...) {
    string id;
    id = remove_article(lower_case(words[0]));
    return trap->eventBoobytrap(this_player(), id, trap);
}
