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

varargs int eventOpen(object who, object tool) {
    if(!this_object()->GetClosed()){
        write("It's already open.");
    }
    else {
        write("This door has no handle or other visible means to open it.");
    }
    return 1;
}

varargs int eventClose(object who) {
    if(this_object()->GetClosed()){
        write("It's already open.");
    }
    else {
        write("This door has no handle or other visible means to close it.");
    }
    return 1;
}

