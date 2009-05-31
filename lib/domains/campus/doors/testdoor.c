#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("east", ([ "id" : ({"door leading east", "door", "east door"}),
                "short" : "a door leading east",
                "long" : "This is the west side of a door leading east.",
                "lockable" : 1 ]) );

    SetSide("west", ([ "id" : ({"door leading west", "door", "west door"}),
                "short" : "a door leading west",
                "long" : "This is the east side of a door leading west.",
                "lockable" : 1 ]) );

    SetClosed(1);
    SetLocked(0);
    SetKeys("east", ({ "east key" }));
    SetKeys("west", ({ "west key" }));
}

void init(){
    ::init();
}
