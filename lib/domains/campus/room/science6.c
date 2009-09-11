#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Secondary Science Hallway West");
    SetLong("This is the secondary hallway in the LPC University Science Building. The hallway runs east from here. To the west is an elevator door.");
    SetClimate("indoors");
    SetExits( ([
                "south" : "/domains/campus/room/stairwell2a",
                "north" : "/domains/campus/room/weaplab",
                "east" : "/domains/campus/room/science5",
                "west" : "/domains/campus/room/shaft1.c",
                ]) );
}

void init(){
    ::init();
}
