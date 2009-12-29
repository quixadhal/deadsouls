#include <lib.h>
#include <medium.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(10);
    SetShort("Elevator Shaft");
    SetLong("Elevator Shaft.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits( ([
                "down" : "/domains/campus/room/shaftb",
                "up" : "/domains/campus/room/shaft1",
                "east" : "/domains/campus/room/basement3.c",
                ]) );
    SetDoor("east","/domains/campus/doors/steel_door3");
    SetMedium(MEDIUM_AIR);
}

void init(){
    ::init();
}
