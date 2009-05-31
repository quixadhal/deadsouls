#include <lib.h>
#include <daemons.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("load");
    SetSynonyms("charge", "recharge");
    SetRules("", "OBJ","OBJ with OBJ","OBJ into OBJ","OBJ in OBJ");
    SetErrorMessage("Load something?");
    SetHelp("Syntax: load THING\n"
            "        load THING on THING\n"
            "Synonyms: charge, recharge\n");  
}

mixed can_load() {
    return this_player()->CanManipulate();
}

mixed can_load_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_load_obj_word_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed do_load() {
    write("Load what?");
    return 1;
}

varargs mixed do_load_obj(mixed foo) {
    return foo->eventLoad();
}

varargs mixed do_load_obj_word_obj(mixed foo, mixed wort, mixed foo2) {
    if(wort == "with"){
        return foo2->eventLoad(this_player(), foo);
    }
    else {
        return foo->eventLoad(this_player(), foo2);
    }
}
