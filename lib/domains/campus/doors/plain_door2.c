#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("east", (["id" : ({ "door", "plain wooden door", "wooden door" }),
                "short" : "a door leading east",
                "long" : "This is a plain wooden door.",
                "lockable" : 0 ]) );
    SetSide("west", (["id" : ({ "door", "plain wooden door", "wooden door" }),
                "short" : "a door leading west",
                "long" : "This is a plain wooden door.",
                "lockable" : 0 ]) );
    SetClosed(1);
    SetLocked(0);
}

void init(){
    ::init();
}
