#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("An alcove");
    SetLong("An alcove.");
    SetClimate("indoors");
    SetExits(([
                "east" : "/domains/campus/room/stairwell2a",
                ]));
}

void init(){
    ::init();
}
