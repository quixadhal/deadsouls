/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 02:39:29 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("front");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("dairy");
    SetLong("This is the home of the milkmaids.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([]));
    SetObviousExits("s");
    AddExit("out", MY_ROOM + "/" +"mstreet6");
    AddExit("south", MY_ROOM + "/" +"mstreet6");



    SetDoor("south", "/domains/fort/doors/dairy.c");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
