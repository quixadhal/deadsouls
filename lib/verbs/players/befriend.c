#pragma save_binary

#include <lib.h>

inherit LIB_VERB;

static void create() {
    ::create();
    SetVerb("befriend");
    SetRules("LIV");
    SetErrorMessage("Whom would you like to befriend?");
    SetHelp("Syntax: befriend LIV\n"
      "\n"
      "If a living object is specified, this command allows "
      "you to try to make friends. This is a prerequisite to " 
      "successfully mounting an animal.\n"
      "\n"
      "");
}

mixed can_befriend_liv() { return 1; }


mixed do_befriend_liv(object ob) {
    ob->eventBefriend(this_player());
    return 1;
}


