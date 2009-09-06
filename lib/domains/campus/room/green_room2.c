#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("a green room");
    SetLong("This is a green room. Probability Lab 2 is east.");
    SetExits( ([
                "east" : "/domains/campus/room/plab2"
                ]) );
    SetDoor("east","/domains/campus/doors/green_door2");
}
void init(){
    ::init();
}
