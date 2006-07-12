/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 12:44:29 2005
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
    SetShort("bailey path");
    SetDayLong("Bailey Path, day");
    SetNightLong("Bailey Path, night.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([]));
    SetObviousExits("n, s, w");
    AddExit("south", MY_ROOM + "/" +"gstreet1");
    AddExit("west", MY_ROOM + "/" +"guildhouse");
    AddExit("north", MY_ROOM + "/" +"bpath2");


}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
