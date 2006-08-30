#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(0);
    SetShort("Dry Spring");
    SetLong("This small, dark chamber appears to have been carved out of the bedrock, and contains many small holes bored into the floor. Considering the difficulty of digging into such solid stuff, this wellwater system must have taken a fortune and a vast amount of labor to construct. A door leads east, up to the water tunnel and the well.");
    SetItems(([
	({ "hole", "holes", "floor" }) : "These holes appear to be where spring water enters to flood the chamber and fill the well east of here. ",
	({ "dirt", "dust" }) : "There's plenty of that here. Empty wells rarely get much priority on cleaning day.",
	({ "bottom", "well" }) : "Dirty, musty, and unpleasant.",
	({ "vermin", "rats", "bugs" }) : "Looks like they're hiding from you at the moment.",
	"haven" : "A nice place for vermin.",
	({ "rock", "bedrock" }) : "This is dense, hard stuff deep underground.",
      ]));
    SetExits(([
	"east" : "/domains/town/room/well2",
      ]));

    SetDoor("east", "/domains/town/doors/welldoor2.c");

}
void init(){
    ::init();
}
