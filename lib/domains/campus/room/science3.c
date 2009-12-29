#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Science Building Hallway, West");
    SetLong("This is the western end of the main hallway in the LPC University Science Building. The main hallway runs east from here, toward the building's exit. Another hallway runs south from here.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits( ([
                "east" : "/domains/campus/room/science2",
                "south" : "/domains/campus/room/science4.c",
                ]) );
}

void init(){
    ::init();
}
