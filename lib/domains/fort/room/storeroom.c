/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Mon Oct 17 17:09:59 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("indoors");
    SetDayLight(30);
    SetNightLight(0);
    SetShort("storeroom");
    SetLong("This is where items are kept before being sold.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([]));
    SetObviousExits("");
    //funs
    //snuf
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
//extras
