#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("south", (["id" : ({ "door", "plain wooden door", "wooden door" }),
                "short" : "a door leading south",
                "long" : "This is a plain wooden door.",
                "lockable" : 0 ]) );
    SetSide("north", (["id" : ({ "door", "plain wooden door", "wooden door" }),
                "short" : "a door leading north",
                "long" : "This is a plain wooden door.",
                "lockable" : 0 ]) );
    SetKeys("north", ({"prob_door_key"}) );
    SetKeys("north", ({"prob_door_key"}) );
    SetClosed(1);
    SetLocked(0);
}
void init(){
    ::init();
}
