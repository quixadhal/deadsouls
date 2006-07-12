/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 02:46:59 2005
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
    SetShort("market square north");
    SetDayLong("Market square, day.");
    SetNightLong("Market square, night.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([  "bakery" : "The fort's bakery is east."]));
    SetInventory(([]));
    SetObviousExits("s, e");
    AddExit("south", MY_ROOM + "/" +"msquare");
    AddExit("east", MY_ROOM + "/" +"bakery");
    AddEnter("bakery", MY_ROOM + "/" +"bakery");


}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
