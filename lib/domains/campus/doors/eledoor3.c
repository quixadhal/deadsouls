#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("east", (["id" : ({ "door", "elevator door" }),
                "short" : "an elevator door leading east",
                "long" : "This is an elevator door.",
                "lockable" : 0 ]) );
    SetSide("west", (["id" : ({ "door", "elevator door" }),
                "short" : "an elevator door leading west",
                "long" : "This is an elevator door.",
                "lockable" : 0 ]) );
    SetClosed(1);
    SetLocked(0);
}

void init(){
    ::init();
}
mixed CanOpen(object ob){
    return "There is no handle to open it.";
}
