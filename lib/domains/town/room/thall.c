#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("town");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Town Hall");
    SetLong("This is the modest foyer of this village's Town Hall. It's a simple affair, really, in keeping with the down-to-earth spirit of the town. The office of the clerk and notary is north, and the Mayor's office is upstairs. Village road is south. The registrar of voters is northeast.");
    SetProperties (([
                "no attack":1, 
                "no bump":1, 
                "no steal":0, 
                "no magic":0, 
                "no paralyze":0, 
                "no teleport":0]));
    SetItems(([]));
    SetExits( ([
                "south" : "/domains/town/room/vill_road3",
                "northeast" : "/domains/town/room/voters",
                "up" : "/domains/town/room/mayor",
                "north" : "/domains/town/room/clerk.c",
                ]) );
    SetInventory(([]));

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
