#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("north", (["id" : ({ "door", "handsome wooden door", "wooden door" }),
                "short" : "a door leading north",
                "long" : "This is a plain wooden door.",
                "lockable" : 1 ]) );

    SetSide("south", (["id" : ({ "door leading into the mansion", "handsome wooden door", "wooden door", "door" }),
                "short" : "a door leading south",
                "long" : "This is a handsome wooden door.",
                "lockable" : 1 ]) );

    SetClosed(1);
    SetLocked(1);
}
void init(){
    ::init();
}
