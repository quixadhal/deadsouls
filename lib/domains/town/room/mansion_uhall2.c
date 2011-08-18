#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Upstairs Mansion Hallway");
    SetLong("You are in the beautifully decorated upstairs "
      "hallway of the mansion. There are rooms to "
      "the east and west, and the hallway continues "
      "north and south."); 
    SetItems( ([
        ({"hall","hallway"}) : "A corridor permitting "+
        "travel from one part of the mansion to another."
      ]) );
    SetExits( ([
        "south" : "/domains/town/room/mansion_uhall3",
        "north" : "/domains/town/room/mansion_uhall1",
        "east" : "/domains/town/room/mansion_room3",
        "west" : "/domains/town/room/mansion_room4"
      ]) );

    SetDoor("west", "/domains/town/doors/m4.c");

    SetDoor("east", "/domains/town/doors/m3.c");

}
void init(){
    ::init();
}
