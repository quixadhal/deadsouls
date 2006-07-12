/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 02:21:21 2005
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
    SetShort("market square");
    SetDayLong("Market square, day.");
    SetNightLong("Market square, night.\n12");
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
    AddExit("west", MY_ROOM + "/" +"msquare_w");
    AddExit("north", MY_ROOM + "/" +"msquare_n");
    AddExit("south", MY_ROOM + "/" +"msquare_s");
    AddExit("east", MY_ROOM + "/" +"mstreet6");


}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
