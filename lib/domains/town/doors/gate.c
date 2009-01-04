#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("northeast", ([ "id" : ({"gate leading northeast", "gate", "northeast gate"}),
                "short" : "a gate leading northeast",
                "long" : "This is the southwest side of a gate leading northeast.",
                "lockable" : 1 ]) );

    SetSide("southwest", ([ "id" : ({"gate leading southwest", "gate", "southwest gate"}),
                "short" : "a gate leading southwest",
                "long" : "This is the northeast side of a gate leading southwest.",
                "lockable" : 1 ]) );

    SetPerforated(1);
    SetClosed(1);
    SetLocked(1);
    SetLockStrength(90);
    SetKeys("northeast", ({ "cavetroll key" }));
    SetKeys("southwest", ({ "cavetroll key" }));
    SetOpacity(0);
}

void init(){
    ::init();
}
