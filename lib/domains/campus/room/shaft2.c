#include <lib.h>
#include <medium.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(10);
    SetShort("elevator shaft");
    SetLong("Elevator Shaft.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits( ([
                "east" : "/domains/campus/room/science7",
                ]) );
    SetSinkRoom("/domains/campus/room/shaft1");
    SetDoor("east", "/domains/campus/doors/eledoor2");
    SetMedium(MEDIUM_AIR);
}
void init(){
    ::init();
}
