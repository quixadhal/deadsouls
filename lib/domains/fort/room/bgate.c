/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 13:13:16 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(25);
    SetShort("bailey gates");
    SetDayLong("The gates to the inner bailey, daytime.");
    SetNightLong("The gates to the inner bailey, nighttime.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([]));
    SetObviousExits("n, s");
    AddExit("south", MY_ROOM + "/" +"bpath5");
    AddExit("north", MY_ROOM + "/" +"common16");


}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
