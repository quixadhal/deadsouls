#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Science Classroom");
    SetLong("This small room provides a teaching space for the advanced topics researched by the facility.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits(([
                "south" : "/domains/campus/room/science5",
                ]));
}

void init(){
    ::init();
}
