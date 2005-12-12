#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Shore Edge");
    SetLong("You are on the shore, at the far eastern end of town. "
      "A road travels west into the heart of town. ");
    SetExits( ([
	"west" : "/domains/town/room/vill_road3",
      ]) );
    SetObviousExits("w");
}
