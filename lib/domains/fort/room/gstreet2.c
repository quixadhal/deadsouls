/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 12:35:47 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(25);
    SetShort("guild street");
    SetDayLong("Guild street 2, day.");
    SetNightLong("Guild street 2, night.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([]));
    SetObviousExits("n, s, e, w");
    AddExit("north", MY_ROOM + "/" +"bpath3");
    AddExit("south", MY_ROOM + "/" +"alley");
    AddExit("west", MY_ROOM + "/" +"gstreet3");
    AddExit("east", MY_ROOM + "/" +"gstreet1");


}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
