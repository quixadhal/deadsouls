#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

static private object *old;

void create() {
    ::create();
    SetProperty("no teleport", 1);
    SetShort( "The freezer");
    SetLong( "The local freezer.");
    SetExits( 
            (["square" : "/domains/Praxis/square"]) );
    SetNoClean(1);
    call_out("clean_room", MAX_NET_DEAD_TIME);
}

static void clean_room() {
    object *in_here, *to_clean;
    int i;

    if(!sizeof(in_here = livings() & all_inventory(this_object()))) { 
        old = in_here;
        call_out("clean_room", MAX_NET_DEAD_TIME);
        return;
    }
    i = sizeof(to_clean = in_here & (pointerp(old) ? old : ({})));
    old = in_here - to_clean;
    while(i--) to_clean[i]->clean_net_dead();
    i = sizeof(to_clean = all_inventory(this_object()) - in_here);
    while(i--) to_clean[i]->destruct();
    call_out("clean_room", MAX_NET_DEAD_TIME);
}
void init(){
    ::init();
}
