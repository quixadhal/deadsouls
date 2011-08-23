#include <lib.h>
#include <medium.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(10);
    SetShort("Elevator shaft");
    SetLong("Elevator shaft, first floor");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits( ([
                "east" : "/domains/campus/room/science6",
                ]) );
    SetDoor("east", "/domains/campus/doors/eledoor1");
    SetFlyRoom("/domains/campus/room/shaft2");
    SetSinkRoom("/domains/campus/room/shaft0");
    SetMedium(MEDIUM_AIR);
}
void init(){
    ::init();
}
