//  /domains/stanadard/obj/mon/execution.c
//  Execution functions for the mortal law system
//  Created 940917 by Manny@Nightmare

#include <lib.h>
#include <dirs.h>

inherit DAEMON;

void create() {
    daemon::create();
}

void stone_player(object who, string where) {
    object me;

    me = previous_object();
    message("say", "%^RED%^"+me->query_cap_name()+", cheers wildly and "
            "runs off to join in the stoning!", environment(me));
    me->eventMoveLiving(where, "cheers wildly and "
            "runs off to join in the stoning!");
} 
