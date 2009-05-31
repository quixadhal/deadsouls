#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(25);
    SetShort("Gardener's Shack");
    SetLong("This is the dark, wooden shack of the keeper "
            "of the garden. From the dust and rust, "
            "it is obvious nobody has done much gardening "
            "around here lately.");
    SetExits( ([
                "south" : "/domains/town/room/garden",
                "out" : "/domains/town/room/garden.c",
                ]) );
    SetInventory(([
                "/domains/town/obj/rayovac" : 1,
                "/domains/town/obj/ladder" : 1,
                ]) );

}
void init(){
    ::init();
}
