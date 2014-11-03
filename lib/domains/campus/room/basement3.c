#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Secret Lab");
    SetLong("This tidy workspace between campus levels seems to "+
            "be some kind of secret laboratory.");
    SetClimate("indoors");
    SetItems( ([ 
                ]) );
    SetInventory(([
                "/domains/campus/weap/zpem" : 1,
                ]));
    SetExits( ([
                "west" : "/domains/campus/room/shaft0",
                "east" : "/domains/campus/room/basement4.c",
                ]) );
    SetDoor("west","/domains/campus/doors/steel_door3");
}

void init(){
    ::init();
}
