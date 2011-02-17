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
                "up" : "/domains/campus/room/shaft0",
                "east" : "/domains/campus/room/sub_basement2.c",
                ]) );
}
void init(){
    ::init();
}
