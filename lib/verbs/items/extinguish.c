#include <lib.h>
#include "include/extinguish.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("extinguish");
    SetSynonyms("douse");
    SetRules("OBS");
    SetErrorMessage("Extinguish what?");
    SetHelp("Syntax: <extinguish OBJECT>\n\n"
            "Extinguish a burning thing like a torch or a lamp.\n\n"
            "See also: light");
}

mixed can_extinguish_obj() {
    return this_player()->CanManipulate();
}

mixed do_extinguish_obj(object target) {
    return do_extinguish_obs(({ target }));
}

mixed do_extinguish_obs(mixed *targs) {
    object *obs;
    string tmp;

    if( !sizeof(targs) ) {
        this_player()->eventPrint("There is no such thing to be extinguished.");
        return 1;
    }
    obs = filter(targs, (: objectp :));
    if( !sizeof(obs) ) {
        mixed *ua;

        ua = unique_array(targs, (: $1 :));
        foreach(string *list in ua) this_player()->eventPrint(list[0]);
        return 1;
    }
    obs = filter(obs, (: $1->eventExtinguish(this_player()) :));
    if( !sizeof(obs) ) return 1;
    tmp = item_list(obs);
    this_player()->eventPrint("You extinguish " + tmp + ".");
    environment(this_player())->eventPrint(this_player()->GetName() +
            " extinguishes " + tmp + ".",
            this_player());
    return 1;
}

