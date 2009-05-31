#include <lib.h>
#include "include/strike.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("strike");
    SetRules("OBJ");
    SetErrorMessage("Strike what?");
    SetHelp("Syntax: <strike OBJECT>\n\n"
            "Certain objects are flammable simply by striking them.  "
            "When you strike such objects, they have a chance of "
            "catching fire and burning.\n\n"
            "See also: burn, douse, light");
}

mixed can_strike_obj() {
    return this_player()->CanManipulate();
}

mixed do_strike_obj(object ob) {
    return ob->eventStrike(this_player());
}
