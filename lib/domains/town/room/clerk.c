#include <lib.h>
inherit LIB_CHAPEL;

static void create() {
    chapel::create();
    SetTown("town");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Town Clerk");
    SetLong("This small office is where people come to conduct their official public\nbusiness. The main area of the town hall is east.");
    SetProperties (([
	"no attack":1, 
	"no bump":1, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([
	"/domains/town/npc/clerk" : 1,
      ]));
    SetObviousExits("e");
    AddExit("east", "/domains/town/room/thall");
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
