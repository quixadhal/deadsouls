#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Dr. Kleiner's Office");
    SetLong("The place where Dr. Kleiner conducts administrative "+
            "business.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits( ([
                "south" : "/domains/campus/room/science7",
                "east" : "/domains/campus/room/klab.c",
                ]) );
}

void init(){
    ::init();
}
