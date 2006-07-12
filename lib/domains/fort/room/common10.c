/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 14:13:12 2005
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
    SetObviousExits("n, s, e, w, ne, se, sw");
    AddExit("southwest", MY_ROOM + "/" +"common16");
    AddExit("west", MY_ROOM + "/" +"bastion");
    AddExit("east", MY_ROOM + "/" +"common11");
    AddExit("north", MY_ROOM + "/" +"common5");
    AddExit("northeast", MY_ROOM + "/" +"common6");
    AddExit("south", MY_ROOM + "/" +"common17");
    AddExit("southeast", MY_ROOM + "/" +"common18");
    //funs
    //snuf
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
//extras
