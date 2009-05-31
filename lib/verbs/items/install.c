#include <lib.h>
#include <daemons.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("install");
    SetRules("", "OBJ","OBJ on OBJ","OBJ into OBJ","OBJ in OBJ");
    SetErrorMessage("Install something?");
    SetHelp("Syntax: install THING\n"
            "        install THING on THING\n"
            "");  
}

mixed can_install() {
    return this_player()->CanManipulate();
}

mixed can_install_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed can_install_obj_word_obj(mixed foo) {
    return this_player()->CanManipulate();
}

mixed do_install() {
    return "Install what?";
    return 1;
}

varargs mixed do_install_obj(mixed foo) {
    return foo->eventInstall();
}

varargs mixed do_install_obj_word_obj(mixed foo, mixed wort, mixed foo2) {
    return foo->eventInstall(this_player(), foo2);
}
