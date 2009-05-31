#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("DIR_X", ([ "id" : ({"door leading DIR_X", "door", "DIR_X door"}),
                "short" : "a door leading DIR_X",
                "long" : "This is the Y_SIDE side of a door leading DIR_X.",
                "lockable" : 1 ]) );

    SetSide("DIR_Y", ([ "id" : ({"door leading DIR_Y", "door", "DIR_Y door"}),
                "short" : "a door leading DIR_Y",
                "long" : "This is the X_SIDE side of a door leading DIR_Y.",
                "lockable" : 1 ]) );

    SetClosed(1);
    SetLocked(0);
    SetKeys("DIR_X", ({ "DIR_X key" }));
    SetKeys("DIR_Y", ({ "DIR_Y key" }));
}

void init(){
    ::init();
}
