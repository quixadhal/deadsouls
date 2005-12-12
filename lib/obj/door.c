#include <lib.h>

inherit LIB_DOOR;

static void create() {
    door::create();
    SetSide("north", ([ "id" : ({"door"}),
	"short" : "a door leading north",
	"long" : "This is the south side of a generic door.",
	"lockable" : 1 ]) );
    SetKeys("north", ({ "mansion key" }));
    SetSide("south", ([ "id" : ({"door"}),
	"short" : "a door leading south",
	"long" : "This is the north side of a generic door.",
	"lockable" : 1 ]) );
    SetClosed(1);
    SetLocked(0);
}
