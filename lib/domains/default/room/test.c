#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The start room");
    SetLong("The default start room. To enter "+
            "a sample set of rooms, go down.");
    SetExits( ([ 
                "down" : "/domains/town/room/road",
                ]) );
}
void init(){
    ::init();
}
