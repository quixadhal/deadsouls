/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 13:31:11 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(15);
    SetShort("The commons");
    SetDayLong("The commons, day.");
    SetNightLong("The commons, night.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([]));
    SetObviousExits("n, s, ne, nw, se, sw");
    AddExit("northwest", MY_ROOM + "/" +"common1");
    AddExit("north", MY_ROOM + "/" +"common2");
    AddExit("southeast", MY_ROOM + "/" +"common15");
    AddExit("southwest", MY_ROOM + "/" +"common13");
    AddExit("northeast", MY_ROOM + "/" +"common3");
    AddExit("south", MY_ROOM + "/" +"common14");
    //funs
    //snuf
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
//extras
