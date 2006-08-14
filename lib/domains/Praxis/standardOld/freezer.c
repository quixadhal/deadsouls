#include <std.h>
#include <config.h>
#include <rooms.h>

inherit ROOM;

static private object *old;

void create() {
    ::create();
    set_property("no teleport", 1);
    set_short( "The freezer");
    set_long( "The local freezer.");
    set_exits( 
      (["square" : "/domains/Praxis/square"]) );
    set_no_clean(1);
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
    while(i--) to_clean[i]->remove();
    call_out("clean_room", MAX_NET_DEAD_TIME);
}
