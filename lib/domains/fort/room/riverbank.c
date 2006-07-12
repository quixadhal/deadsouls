/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Fri Oct 28 02:29:09 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
inherit LIB_ROOM;

static void create() {
    room::create();
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
    SetInventory(([]));
    SetObviousExits("up");
    AddExit("up", "/domains/town/room/bridge");
    //funs
    //snuf
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([  "default" : "You can hear the roar of the river rushing by."]));
}
//extras
