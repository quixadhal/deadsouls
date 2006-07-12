/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Mon Oct 17 19:28:37 2005
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
    SetShort("north gate tower");
    SetDayLong("You are at the top of the north gate tower. This is where the fortress militia patrol the battlements to guard against invaders. The view is remarkable here, not only of the gate immediately below, but of the great northeastern forest. The south gate tower is south of here. The unusual design of the fort prevents travel from here to the northern battlements: the inner bailey is a kind of fort inside the fort that provides a final defensive position. It is not accessible from here, in case the outer walls fall into enemy hands.");
    SetItems(([
	"tower" : "This is one of the towers that flank the main gate.",
	({ "view", "forest", "gate", "northeastern forest", "great northeastern forest" }) : "The view from up here is simply breathtaking.",
	({ "battlement", "battlements" }) : "These are defensive structures along the top of the wall that provide cover for defenders.",
	({ "bailey", "inner bailey", "inner fort" }) : "The fort's east wall is not navigable north from here, preventing access to the inner bailey. The inner bailey is north, and you can see it is a kind of fort within the fort, designed as a last defense in the desperate case of a successful invasion of the outer walls.",
	({ "wall", "walls" }) : "These walls appear extremely strong and thick, as one would expect of a military fortification.",
      ]));
    SetInventory(([
	"/domains/fort/npc/tower_guard" : 1,
      ]));
    SetNightLong("You are at the top of the north gate tower. This is where the fortress militia patrol the battlements to guard against invaders. The south gate tower is south of here. The unusual design of the fort prevents travel from here to the northern battlements: the inner bailey is a kind of fort inside the fort that provides a final defensive position. It is not accessible from here, in case the outer walls fall into enemy hands.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetObviousExits("s");
    AddExit("south", MY_ROOM + "/" +"south_gtower_top");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}