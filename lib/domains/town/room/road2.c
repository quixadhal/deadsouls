#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("West Village road, west");
    SetLong("This is a well-traveled road, leading "
      "east into town and west away from it. An old, "
      "humpbacked bridge is west of here.");
    SetExits( ([
	"east" : "/domains/town/room/road1",
	"west" : "/domains/town/room/bridge",
      ]) );

    SetObviousExits("e,w");
}
