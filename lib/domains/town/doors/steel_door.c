#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("east", (["id" : ({ "door", "east door" }),
                "short" : "a steel door leading east",
                "long" : "This is an imposing, large steel door leading east into the arena.",
                "lockable" : 0 ]) );
    SetSide("west", (["id" : ({ "west door", "door" }),
                "short" : "a steel door leading west",
                "long" : "This is an imposing, large steel door leading west, out of the arena.",
                "lockable" : 0 ]) );

    SetClosed(0);
    SetLocked(0);
}
void init(){
    ::init();
}
