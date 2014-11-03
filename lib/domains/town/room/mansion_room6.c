#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Servants' Quarters");
    SetLong("You are in the spartan quarters of the on-duty "
            "staff. From the lack of personalization, "
            "it seems that the servants probably rotate "
            "after-hours duty.");
    SetInventory( ([
                "/domains/town/obj/cot" :1,
                ]) );
    SetExits( ([
                "east" : "/domains/town/room/mansion_uhall3"
                ]) );

    SetDoor("east", "/domains/town/doors/m6.c");

}
void init(){
    ::init();
}
