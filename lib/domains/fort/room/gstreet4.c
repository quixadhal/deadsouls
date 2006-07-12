/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 12:31:51 2005
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
    SetDayLong("Guild street, day.");
    SetNightLong("Guild street, night.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([]));
    SetObviousExits("n, e, w");
    AddExit("west", MY_ROOM + "/" +"gstreet5");
    AddExit("north", MY_ROOM + "/" +"carpenter");
    AddExit("east", MY_ROOM + "/" +"gstreet3");


}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
