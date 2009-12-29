#include <lib.h>

inherit LIB_VERB;

static void create() {
    ::create();
    SetVerb("abandon");
    SetRules("LIV");
    SetErrorMessage("Whom would you like to abandon?");
    SetHelp("Syntax: abandon <LIV>\n\n"
            "If a living object is specified, this command allows "
            "you to leave the creature's companionship.\n" 
            "See also: mount, dismount, befriend");
}

mixed can_abandon_liv() { return 1; }

mixed do_abandon_liv(object ob) {
    ob->eventAbandon(this_player());
    return 1;
}
