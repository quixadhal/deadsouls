#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Secret Lab");
    SetLong("This room seems to be used for secret experiments.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetExits(([
                "west" : "/domains/campus/room/basement3",
                ]));
}

void init(){
    ::init();
}
