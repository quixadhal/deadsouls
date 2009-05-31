#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Science Building Hallway");
    SetLong("This is the LPC University Science Building, where exciting new systems and abstract research happens. South of here is the probability laboratory. The hallway continues west. The building's exit is north, leading to University Square.");
    SetExits( ([ 
                "north" : "/domains/campus/room/usquare",
                "south" : "/domains/campus/room/wiz_lab",
                "west" : "/domains/campus/room/science2",
                ]) );
    SetClimate("indoors");

    SetDoor("south", "/domains/campus/doors/prob_door.c");

}
void init(){
    ::init();
}
