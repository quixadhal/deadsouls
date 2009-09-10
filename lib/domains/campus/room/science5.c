#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Secondary Science Hallway East");
    SetLong("This is the secondary hallway in the LPC University Science Building. The hallway runs east and west from here. North of here is a classroom.");
    SetClimate("indoors");
    SetExits( ([
        "north" : "/domains/campus/room/classroom1",
        "east" : "/domains/campus/room/science4",
        "west" : "/domains/campus/room/science6",
        "south" : "/domains/campus/room/medlab.c",
      ]) );

}
void init(){
    ::init();
}
