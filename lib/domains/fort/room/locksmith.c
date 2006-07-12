/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 02:41:13 2005
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
    SetShort("locksmith");
    SetLong("This is the locksmith's shop.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([]));
    SetObviousExits("n");
    AddExit("out", MY_ROOM + "/" +"mstreet6");
    AddExit("north", MY_ROOM + "/" +"mstreet6");
    SetDoor("north", "/domains/fort/doors/locksmith.c");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
