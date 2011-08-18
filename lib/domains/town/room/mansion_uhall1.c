#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Upstairs Mansion Hallway North");
    SetLong("You are in the beautifully decorated upstairs "
      "hallway of the mansion. There are rooms to "
      "the east and west, and the hallway continues "
      "south. An open window at the end of the hall "
      "leads down.");
    SetItems( ([
        ({"window","open window"}) : "An open window here "+
        "leads down to the exterior of the mansion.",
        ({"hall","hallway"}) : "A corridor permitting "+
        "travel from one part of the mansion to another."
      ]) );
    SetExits( ([
        "down" : "/domains/town/room/mansion_ext",
        "south" : "/domains/town/room/mansion_uhall2",
        "east" : "/domains/town/room/mansion_room1",
        "west" : "/domains/town/room/mansion_room2",
        "window" : "/domains/town/room/mansion_ext"
      ]) );
    SetEnters( ([
        "window" : "/domains/town/room/mansion_ext"
      ]) );

    SetDoor("east", "/domains/town/doors/m1.c");

    SetDoor("west", "/domains/town/doors/m2.c");

}
void init(){
    ::init();
}
