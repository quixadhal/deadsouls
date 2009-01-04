//  /domains/Praxis/obj/misc/handcuffs.c
//  From the Nightmare IV mudlib
//  Autoloading handcuffs for use with the mortal law system.
//  Created by Manny@Nightmare 940821

#include <lib.h>
#include <dirs.h>
#define ALLOWED_COMMANDS ({ "look", "say", "tell", "shout", "smile",\
        "frown", "grin", "shiver", "giggle", "laugh", "cry", "whimper",\
        "inventory", "pray", "dest", "call", "eval" })

inherit LIB_ITEM;

create() {
    ::create();
    SetId( ({ "handcuffs", "cuffs" }) );
    SetKeyName("handcuffs");
    SetShort("Rope handcuffs");
    SetLong("Thick rope used to restrain unlawful characters.");
    SetMass(0);
    SetValue(0);
    SetPreventDrop("You're unable to drop that, you're hands are tied.");
}

void init() {
    ::init();
    add_action("all_cmds", "");
    if(!living(environment(this_object()))) this_object()->eventDestruct();
    command("save");
}

mixed *query_auto_load() {
    return (DIR_STANDARD_DOMAIN+"/obj/misc/handcuffs.c", ({ }) );
}

int all_cmds(string str) {

    if(member_array(query_verb(), ALLOWED_COMMANDS) == -1) {
        message("say", "%^RED%^You cannot do that with the handcuffs on.",
                this_player());
        return 1;
    }
    if(this_player()->query_ghost()) call_out("destroy_me", 0);
    return 0;
}

void destroy_me() {
    eventDestruct();
}
