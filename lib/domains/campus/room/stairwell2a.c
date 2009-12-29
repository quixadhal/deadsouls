#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Science Building Stairwell, First floor");
    SetLong("This is the first floor stairwell of the University Science Building.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits( ([
                "north" : "/domains/campus/room/science6",
                "west" : "/domains/campus/room/alcove1",
                "up" : "/domains/campus/room/stairwell2b",
                "down" : "/domains/campus/room/stairwell2c.c",
                ]) );
}

void init(){
    ::init();
}
