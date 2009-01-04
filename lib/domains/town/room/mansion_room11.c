#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Secret Room");
    SetLong("You are in a small, cramped room.");
    SetExits( ([
                "out" : "/domains/town/room/mansion_mbdroom",
                "north" : "/domains/town/room/mansion_mbdroom"
                ]) );
    SetInventory( ([
                "/domains/town/obj/safe" : 1,
                ]) );
}
void init(){
    ::init();
}
