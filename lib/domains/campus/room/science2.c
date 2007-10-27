#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Science Building Hallway, West");
    SetLong("This is the western end of the main hallway in the LPC University Science Building. North of here is the stargate laboratory. The hallway runs east from here, toward the building's exit.");
    SetExits( ([
        "north" : "/domains/campus/room/slab",
        "east" : "/domains/campus/room/science1",
        "south" : "/domains/campus/room/hazlab.c",
      ]) );

    SetClimate("indoors");

}
void init(){
    ::init();
}
