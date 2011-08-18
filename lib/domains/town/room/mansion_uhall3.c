#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Upstairs Mansion Hallway South");
    SetLong("You are in the beautifully decorated upstairs "
      "hallway of the mansion. There are rooms to "
      "the east and west, and the hallway continues "
      "north. The master bedroom is south. A large, "
      "circular marble staircase leads down to the "
      "ground floor."); 
    SetItems( ([
        ({"hall","hallway"}) : "A corridor permitting "+
        "travel from one part of the mansion to another."
      ]) );

    SetExits( ([
        "south" : "/domains/town/room/mansion_mbdroom",
        "north" : "/domains/town/room/mansion_uhall2",
        "down" : "/domains/town/room/mansion_dhall3",
        "east" : "/domains/town/room/mansion_room5",
        "west" : "/domains/town/room/mansion_room6"
      ]) );

    SetDoor("east", "/domains/town/doors/m5.c");

    SetDoor("west", "/domains/town/doors/m6.c");

    SetDoor("south", "/domains/town/doors/m7.c");

}
void init(){
    ::init();
}
