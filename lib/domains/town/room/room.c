#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("");
    SetLong("");
    SetExits( ([
	"out" : "/domains/town/room/mansion_room3"
      ]) );
    SetObviousExits("out");
}
