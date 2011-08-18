#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Upstairs Bathroom");
    SetLong("You are in a bathroom. It is rather ordinary, "
      "for such a grand mansion, but it appears "
      "quite functional and mostly clean. There is "
      "a shower stall here you can enter.");
    SetItems( ([
        ({"stall","shower stall","shower" })  : "A shower stall.",
        ({"sink","bathroom sink" })  : "An ordinary sink.",
      ]) );
    SetExits( ([
        "west" : "/domains/town/room/mansion_uhall2"
      ]) );
    SetInventory( ([
        "/domains/town/obj/toilet" : 1,
        "/domains/town/obj/mat" : 1
      ]) );
    SetEnters( ([
        "stall" : "/domains/town/room/shower",
        "shower stall" : "/domains/town/room/shower",
        "shower" : "/domains/town/room/shower"
      ]) );

    SetDoor("west", "/domains/town/doors/m3.c");

}
void init(){
    ::init();
}
