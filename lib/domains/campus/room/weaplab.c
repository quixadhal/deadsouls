#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Weapons Lab");
    SetLong("Weapons are tested here.");
    SetClimate("indoors");
    SetProperty("nopeer",1);
    SetItems( ([ 
                ]) );
    SetInventory(([
                "/domains/default/weap/prifle" : 1,
                "/domains/default/armor/pcannon" : 1,
                ]));
    SetExits( ([
                "south" : "/domains/campus/room/science6",
                "west" : "/domains/campus/room/alcove2.c",
                ]) );
}

void init(){
    ::init();
}
