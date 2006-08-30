#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(0);
    SetShort("Town Well");
    SetLong("This is the bottom of the old town well. It is quite dark and unpleasant down here. Years of disuse have provided a haven for vermin, dirt, and dust here. A service door of some kind is set in the west wall here.\n"+
      "%^GREEN%^There is a lever set in the wall here.%^RESET%^");
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
    SetInventory(([
	"/domains/town/obj/well_lever" : 1,
      ]));
    SetExits( ([ 
	"up" : "/domains/town/room/south_road1",
	"west" : "/domains/town/room/well2",
      ]) );

    SetDoor("west", "/domains/town/doors/welldoor1.c");

}
void init(){
    ::init();
}
