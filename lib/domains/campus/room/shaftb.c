#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(10);
    SetShort("Elevator Shaft");
    SetLong("Elevator shaft, basement");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits( ([
                "east" : "/domains/campus/room/sub_basement2.c",
                ]) );
    SetFlyRoom("/domains/campus/room/shaft0");
}
void init(){
    ::init();
}
