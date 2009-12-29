#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetShort("hallway bend");
    SetLong("This is a bend in the hallway. The main Science Building hallway is north of here. A smaller hallway runs west from here.");
    SetClimate("indoors");
    SetExits( ([
                "west" : "/domains/campus/room/science5",
                "north" : "/domains/campus/room/science3.c",
                ]) );
}

void init(){
    ::init();
}
