/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 13:16:19 2005
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
    SetObviousExits("s, e, w, ne, nw");
    AddExit("east", MY_ROOM + "/" +"common17");
    AddExit("south", MY_ROOM + "/" +"bgate");
    AddExit("northwest", MY_ROOM + "/" +"common9");
    AddExit("northeast", MY_ROOM + "/" +"common10");
    AddExit("west", MY_ROOM + "/" +"common15");


}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
