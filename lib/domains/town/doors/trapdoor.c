#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("down", (["id" : ({ "trapdoor on the floor", "trap door on the floor", "trap door", "trapdoor", "door" }),
                "short" : "a trapdoor on the floor, leading downward",
                "long" : "a trapdoor on the floor, leading downward.",
                "lockable" : 0 ]) );
    SetSide("up", (["id" : ({ "trapdoor on the ceiling", "trap door on the ceiling", "trap door", "trapdoor", "door" }),
                "short" : "a trapdoor in the ceiling, leading upward",
                "long" : "a trapdoor in the ceiling, leading upward.",
                "lockable" : 0 ]) );

    SetClosed(1);
    SetLocked(0);
}
void init(){
    ::init();
}
