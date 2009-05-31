#include <lib.h>
#include ROOMS_H

inherit LIB_NPC;

private object __Target;

void heart_beat() {
    ::heart_beat();
    if(__Target && environment(__Target) != environment(this_object())) {
        message("say", "%^BOLD%^%^RED%^The guard tells you: %^RESET%^"+
                "An' where do ya think you're goin' buster??", __Target);
        __Target->eventMoveLiving(environment(this_object()));
    }
}

void set_target(object ob) {
    if(!ob) return;
    __Target = ob;
}
