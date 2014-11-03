#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("down", ([ "id" : ({"door leading down", "door", "down door"}),
                "short" : "a door leading down",
                "long" : "This is the up side of a door leading down.",
                "lockable" : 1 ]) );

    SetSide("up", ([ "id" : ({"door leading up", "door", "up door"}),
                "short" : "a door leading up",
                "long" : "This is the down side of a door leading up.",
                "lockable" : 1 ]) );

    SetClosed(1);
    SetLocked(0);
    SetKeys("down", ({ "down key" }));
    SetKeys("up", ({ "up key" }));
}

void init(){
    ::init();
}
