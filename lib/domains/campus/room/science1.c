#include <lib.h>
#include <rooms.h>

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Science Building Hallway");
    SetLong("This is the LPC University Science Building, where exciting new systems and abstract research happens. North of here is the probability laboratory. The hallway continues west. The building's exit is east, leading to University Square.");
    SetExits( ([
        "north" : "/domains/campus/room/wiz_lab",
        "east" : "/domains/campus/room/usquare",
        "west" : "/domains/campus/room/science2.c",
      ]) );
    SetClimate("indoors");

    SetDoor("north", "/domains/campus/doors/prob_door.c");

}
void init(){
    ::init();
}
