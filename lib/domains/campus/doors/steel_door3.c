#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("east", (["id" : ({ "door", "steel door", "steely door" }),
                "short" : "a steel door leading east",
                "long" : "This is a steel door.",
                "lockable" : 1 ]) );
    SetSide("west", (["id" : ({ "door", "steel door", "steely door" }),
                "short" : "a steel door leading west",
                "long" : "This is a steel door.",
                "lockable" : 1 ]) );
    SetKeys("north", ({"shaft_door_key"}) );
    SetKeys("north", ({"shaft_door_key"}) );
    SetClosed(1);
    SetLocked(1);
}

void init(){
    ::init();
}
