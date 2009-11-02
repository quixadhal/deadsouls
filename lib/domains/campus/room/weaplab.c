#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Weapons Lab");
    SetLong("Weapons are tested here.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits( ([
                "south" : "/domains/campus/room/science6",
                "west" : "/domains/campus/room/alcove2.c",
                ]) );
}

void init(){
    ::init();
}
