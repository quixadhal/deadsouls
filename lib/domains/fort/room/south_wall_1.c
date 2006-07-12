/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 14:46:36 2005
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
    SetShort("south wall");
    SetDayLong("You are at the top of the south wall. This is where the fortress militia patrol to keep an eye on the southern marshes. The battlements of this wall run east to the main watchtower, and west toward the southwest tower.");
    SetNightLong("You are at the top of the south wall. This is where the fortress militia patrol to keep an eye on the southern marshes. The battlements of this wall run east and west into darkness.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([      ({"wall", "walls"}) : "These walls appear extremely strong and thick, as one would expect of a military fortification." ,
	({"battlement", "battlements"}) : "These are defensive structures along the top of the wall that provide cover for defenders."]));
    SetInventory(([]));
    SetObviousExits("e, w");
    AddExit("west", MY_ROOM + "/" +"south_wall_2");
    AddExit("east", MY_ROOM + "/" +"watchtower");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}