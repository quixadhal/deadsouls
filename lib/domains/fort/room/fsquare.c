/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 03:05:14 2005
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
    SetShort("fountain square");
    SetDayLong("Fountain square, day.");
    SetNightLong("Fountain square, night.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([  ({"tavern", "pub", "bar"}) : "Bibi's Tavern is east of here." ,
	"inn" : "The Log Right Inn is north."]));
    SetInventory(([]));
    SetObviousExits("n, s, e");
    AddExit("south", MY_ROOM + "/" +"msquare_w");
    AddExit("east", MY_ROOM + "/" +"pub");
    AddExit("north", MY_ROOM + "/" +"inn");
    AddEnter("tavern", MY_ROOM + "/" +"pub");
    AddEnter("bar", MY_ROOM + "/" +"pub");
    AddEnter("pub", MY_ROOM + "/" +"pub");
    AddEnter("inn", MY_ROOM + "/" +"inn");


}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
