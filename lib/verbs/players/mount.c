#include <lib.h>
#include <daemons.h>
#include <function.h>

inherit LIB_VERB;

static void create() {
    verb::create();
    SetVerb("mount");
    SetRules("LIV");
    SetErrorMessage("Mount something?");
    SetHelp("Syntax: mount <CREATURE>\n\n"
            "This command allows you to put yourself astride a "
            "creature suitable and ready for riding.\n"
            "See also: dismount, befriend, abandon");
}

mixed can_mount_liv() {
    if(this_player()->CanManipulate()){ 
        return bool_reverse(this_player()->GetParalyzed());
    }
}

mixed do_mount_liv(object ob) {
    return ob->eventMount(this_player());
}
