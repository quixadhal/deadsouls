#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("An alcove");
    SetLong("An alcove.");
    SetClimate("indoors");
    SetItems( ([ 
      ]) );
    SetInventory(([
        "/domains/campus/weap/board" : 1,
      ]));
    SetExits(([
        "east" : "/domains/campus/room/weaplab",
      ]));
}

void init(){
    ::init();
}
