#include <lib.h>
#include <daemons.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("uninstall");
    SetRules("OBJ","OBJ from OBJ","OBJ in OBJ","OBJ on OBJ");
    SetErrorMessage("Uninstall something?");
    SetHelp("Syntax: uninstall THING\n"
            "        uninstall THING from THING\n"
            "");  
}

mixed can_uninstall() {
    return this_player()->CanManipulate();
}

mixed can_uninstall_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_uninstall_obj_word_obj(mixed foo...) {
    return this_player()->CanManipulate();
}

mixed do_uninstall() {
    write("Uninstall what?");
    return 1;
}

varargs mixed do_uninstall_obj(mixed foo) {
    write("Uninstall what from where?");
    return 1;
}

varargs mixed do_uninstall_obj_word_obj(mixed foo, mixed foo2, mixed foo3) {
    return foo->eventUninstall(foo3);
}
