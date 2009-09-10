#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("copy of science6.c");
    SetLong("This is the second floor of the LPC University Science Building. To the west is an elevator door.");
    SetClimate("indoors");
    SetExits( ([
        "north" : "/domains/campus/room/kleiner",
        "south" : "/domains/campus/room/stairwell2b",
        "west" : "/domains/campus/room/shaft2.c",
      ]) );
}

void init(){
    ::init();
}
