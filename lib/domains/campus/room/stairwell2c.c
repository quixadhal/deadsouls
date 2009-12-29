#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Science Building Stairwell");
    SetLong("This is the stairwell of the University Science Building.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits( ([
                "up" : "/domains/campus/room/stairwell2a",
                "down" : "/domains/campus/room/stairwell2d.c",
                ]) );
}

void init(){
    ::init();
}
