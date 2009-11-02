#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("a blue room");
    SetLong("This is a blue room. Probability Lab 2 is east.");
    SetExits( ([
                "east" : "/domains/campus/room/plab2"
                ]) );
    SetDoor("east","/domains/campus/doors/blue_door2");
}

void init(){
    ::init();
}
