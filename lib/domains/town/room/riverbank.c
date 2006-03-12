#include <lib.h>
inherit LIB_FISHING;

static void create() {
    ::create();
    SetTown("town");
    SetClimate("temperate");
    SetAmbientLight(30);
    SetShort("riverbank");
    SetLong("This is the bank of a narrow but swift-moving river. A steep path leads back up to the bridge.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([  "river" : "A swift moving river, probably quite deep." ,
	({"bank","riverbank","here"}) : "This is the edge of a powerful river."]));
    SetInventory(([
	"/domains/town/npc/troll" : 1,
	"/domains/town/obj/rocks" : 1,
      ]));
    AddExit("up", "/domains/town/room/bridge");
    SetChance(90);
    SetFish( ([
	"/domains/town/meals/shark" : 10,
      ]) );
    //funs
    //snuf
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([  "default" : "You can hear the roar of the river rushing by."]));
}