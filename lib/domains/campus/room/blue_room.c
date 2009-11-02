#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("a blue room");
    SetLong("This is a blue room. Probability Lab 1 is south.");
    SetExits( ([
                "south" : "/domains/campus/room/monty"
                ]) );
    SetDoor("south","/domains/campus/doors/blue_door");
}

void init(){
    ::init();
}
