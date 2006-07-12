/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Mon Oct 17 20:02:30 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "/domains/fort/customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(25);
    SetShort("east wall");
    SetDayLong("You are at the top of the east wall. This is where the fortress militia patrol the battlements to guard against invaders. The view is remarkable here, not only of the outside fortress road below, but of the great northeastern forest. The battlements of the fort's eastern wall run north and south of here. The main gate towers are north of here.");
    SetItems(([
	({ "battlement", "battlements" }) : "These are defensive structures along the top of the wall that provide cover for defenders.",
	({ "wall", "walls" }) : "These walls appear extremely strong and thick, as one would expect of a military fortification.",
	({ "view", "road", "forest", "northeastern forest", "great northeastern forest" }) : "The view from up here is simply breathtaking.",
      ]));
    SetNightLong("You are at the top of the east wall. This is where the fortress militia patrol the battlements to guard against invaders. The battlements of the fort's eastern wall run into darkness north and south of here. The main gate towers are north of here.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetInventory(([]));
    SetObviousExits("n, s");
    AddExit("south", MY_ROOM + "/" +"watchtower");
    AddExit("north", MY_ROOM + "/" +"south_gtower_top");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}