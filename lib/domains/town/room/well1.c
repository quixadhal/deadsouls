#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(0);
    SetShort("Town Well");
    SetLong("This is the bottom of the old town well. It is "
      "quite dark and unpleasant down here. Years "
      "of disuse after drainage have provided a "
      "haven for vermin, dirt, and dust here.");
    SetItems( ([
	({ "bottom","well" }) : "Dirty, musty, "
	"and unpleasant.",
	"haven" : "A nice place for vermin.",
	({"dirt","dust"}) : "There's plenty of that "
	"here. Empty wells rarely get much "
	"priority on cleaning day.",
	({"vermin","rats","bugs"}) : "Looks "
	"like they're hiding from you at the "
	"moment."
      ]) );
    SetExits( ([
	"out" : "/domains/town/room/south_road1",
	"up" : "/domains/town/room/south_road1"
      ]) );
    SetObviousExits("up");
}
