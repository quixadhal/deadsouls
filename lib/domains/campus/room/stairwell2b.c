#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Science Building Stairwell, second floor");
    SetLong("This is the second floor stairwell of the University Science Building.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits( ([
                "down" : "/domains/campus/room/stairwell2a",
                "north" : "/domains/campus/room/science7.c",
                ]) );
}

void init(){
    ::init();
}
