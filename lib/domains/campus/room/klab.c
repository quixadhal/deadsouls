#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Keliner storage");
    SetLong("This is where Dr. Kleiner apparently keeps large "+
            "pieces of research equipment.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits(([
                "west" : "/domains/campus/room/kleiner",
                ]));
}

void init(){
    ::init();
}
