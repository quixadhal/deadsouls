#include <lib.h>
#include <config.h>
#include <rooms.h>

inherit LIB_ROOM;

static private object *Old;

void create() {
    room::create();
    SetNoClean(1);
    SetProperties(([ "login" : ROOM_START, "no teleport" : 1 ]));
    SetShort( "The freezer");
    SetLong( "The local freezer.  Go down to leave.");
    SetObviousExits("d");
    SetExits( ([ "down" : ROOM_START ]) );
    Old = ({});
    call_out("clean_room", MAX_NET_DEAD_TIME);
}

static void clean_room() {
    object *clean_me;
    object ob;

    foreach(ob in filter(all_inventory(), (: !living($1) :)))
    ob->eventDestruct();
    if( !sizeof(filter(all_inventory(), (: living :))) ) {
	Old = ({});
	call_out((: clean_room :), MAX_NET_DEAD_TIME);
	return;
    }
    clean_me = (all_inventory() & Old);
    Old = all_inventory() - clean_me;
    foreach(ob in clean_me) ob->eventDestruct();
    call_out((: clean_room :), MAX_NET_DEAD_TIME);
}
