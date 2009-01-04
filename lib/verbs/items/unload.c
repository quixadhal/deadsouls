#include <lib.h>
#include <daemons.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("unload");
    SetSynonyms("discharge","deplete");
    SetRules("OBJ","OBJ from OBJ","OBJ in OBJ","OBJ on OBJ", "OBJ out of OBJ");
    SetErrorMessage("Unload something?");
    SetHelp("Syntax: unload THING\n"
            "        unload THING from THING\n"
            "Synonyms: discharge, deplete\n");  
}

mixed can_unload() {
    return this_player()->CanManipulate();
}

mixed can_unload_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_unload_obj_word_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed do_unload() {
    return "Unload what?";
    return 1;
}

varargs mixed do_unload_obj(mixed foo) {
    return foo->eventUnload(foo);
}

varargs mixed do_unload_obj_word_obj(mixed foo, mixed foo2, mixed foo3) {
    return foo->eventUnload(foo3);
}
