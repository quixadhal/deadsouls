#include <lib.h>
#include "include/use.h"

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("use");
    SetRules("OBJ", "OBJ to STR");
    SetErrorMessage("Use what to do what?");
    SetHelp("Syntax: use OBJ to CMD\n\n"
            "Certain objects, like scrolls, allow you to perform acts beyond "
            "your naturaly abilities.  Of course, the only way to know if an "
            "object can so empower you is either to try it or to somehow "
            "detect its magic.\n\n"
            "See also: detect, discern");
}

mixed can_use_obj(mixed args...){ 
    return 1; 
}

mixed can_use_obj_to_str(mixed args...){ 
    return 1; 
}

mixed do_use_obj_to_str(object ob, string str) {
    if(!ob){
        write("That's not here.");
        return 1;
    }
    return ob->eventUse(this_player(), str);
}

mixed do_use_obj(object ob) {
    return do_use_obj_to_str(ob, 0);
}
