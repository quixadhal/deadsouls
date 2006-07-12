#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();

    SetSide("west", (["id" : ({ "door leading west", "door", "west door" }),
	"short" : "a door leading west",
	"long" : "A groovy door",
	"lockable" : 1 ]) );
    SetKeys("west", ({ "west key" }));

    SetSide("east", (["id" : ({ "door leading east", "door", "east door" }),
	"short" : "a door leading east",
	"long" : "This is the west side of a door leading east.",
	"lockable" : 1 ]) );
    SetKeys("east", ({ "east key" }));

    SetClosed(0);
    SetHiddenDoor(0);
    SetLocked(0);
}
void init(){
    ::init();
}
