#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Mountain Road");
    SetLong("You are on a road running north and south. North is "
      "a wreck of rubble and stones where the road is still in "
      "the process of being built. Far in the distance, beautiful "
      "snow-capped mountains can be seen. The road runs south into "
      "the heart of the town.");
    SetExits( ([
	"south" : "/domains/town/room/road",
      ]) );
    SetItems( ([
	({"rubble","stone","stones","road"}) : "Looks like "
	"the road is still being built. Further travel north "
	"is impossible.",
	({"mountains","beautiful","snow-capped mountains"}) : "Beautiful "
	"mountains, far off in the northern distance."
      ]) );

    SetObviousExits("s");
}
