/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Mon Oct 17 20:30:30 2005
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
    SetShort("main watchtower");
    SetDayLong("You are at the top of the main watchtower. This is the tallest tower of the fortress, and provides the best overwatch of the only avenue of approach to the fort. The fort is built on the highest ground in the area, and the land around it slopes down steeply for many meters, making the southeast Fortress Road the only way to get in. The view is breathtaking here, not only of the great northeastern forest, but also the river running through the southeast marshlands and into the vast eastern sea. Far to the south you see the cluster of buildings that is the Old Village, and surrounding the base of the fortress plateau on southwest, west, and north is thickly wooded wilderness. The fort's battlements lead north here to the east wall, and west to the south wall. Stairs lead down to a guardhouse inside the watchtower.");
    SetItems(([
	({ "battlement", "battlements" }) : "These are defensive structures along the top of the wall that provide cover for defenders.",
	({ "wall", "walls" }) : "These walls appear extremely strong and thick, as one would expect of a military fortification.",
	({ "view", "forest", "ocean", "marshlands", "village" }) : "The view from up here is simply breathtaking.",
	({ "stair", "stairs", "stairwell", "stairway", "steps" }) : "This is a set of stairs that circles the interior wall of the tower, leading down.",
	({ "tower", "watchtower" }) : "This is the highest tower of the fortress, used to surveil the land for hostile forces.",
      ]));
    SetInventory(([
	"/domains/fort/npc/tower_guard" : 1,
      ]));
    SetNightLong("You are at the top of the main watchtower. This is the tallest tower of the fortress, and provides the best overwatch of the only avenue of approach to the fort. The fort's battlements lead north and west here into darkness. Stairs lead down to a guardhouse inside the watchtower.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetObviousExits("n, w, d");
    AddExit("north", MY_ROOM + "/" +"east_wall_south");
    AddExit("down", MY_ROOM + "/" +"guardhouse");
    AddExit("west", MY_ROOM + "/" +"south_wall_1");
    AddEnter("watchtower", MY_ROOM + "/" +"guardhouse");
    AddEnter("tower", MY_ROOM + "/" +"guardhouse");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}