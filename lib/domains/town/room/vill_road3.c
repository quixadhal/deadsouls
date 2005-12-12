#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("East Village Road");
    SetLong("A long road going through the village." +
      "Town Hall is north. The main part of the town "+
      "is west of here. The post office is south.");
    SetExits( ([
	"east" : "/domains/town/room/shore",
	"west" : "/domains/town/room/vill_road2",
	"north" : "/domains/town/room/thall",
	"south" : "/domains/town/room/postoffice",
      ]) );
    SetObviousExits("n, s, e, w");
}
