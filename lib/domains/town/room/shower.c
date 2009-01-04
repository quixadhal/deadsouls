#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Shower Stall");
    SetLong("You are standing in a shower stall. People come "
            "here to get clean.");
    SetExits( ([
                "out" : "/domains/town/room/mansion_room3"
                ]) );
    SetObviousExits("out");
}

void init(){
    ::init();
}
