#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("Medical Research Lab");
    SetLong("Medical experiments occur here.");
    SetClimate("indoors");
    SetProperty("nopeer",1);
    SetItems( ([ 
                ({ "door","sliding door" }) : "A strange, metallic sliding door.",
                ]) );
    SetExits( ([
                "north" : "/domains/campus/room/science5.c",
                ]) );
}

void init(){
    ::init();
}
