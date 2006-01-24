#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("town");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Town Hall");
    SetLong("This is the modest foyer of this village's Town Hall. It's a simple \naffair, really, in keeping with the down-to-earth spirit of the town. \nThe office of the clerk and notary is west, and the Mayor's office is north. Village road is south. The registrar of voters is east.");
    SetProperties (([
	"no attack":1, 
	"no bump":1, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetExits( ([
	"north" : "/domains/town/room/mayor",
	"east" : "/domains/town/room/voters",
	"south" : "/domains/town/room/vill_road3",
	"west" : "/domains/town/room/clerk",
	"out" : "/domains/town/room/vill_road3.c",
      ]) );
    SetInventory(([]));


}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
