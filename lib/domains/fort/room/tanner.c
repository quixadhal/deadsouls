/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 15:30:47 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("tanner");
    SetLong("Tanner's shop.");
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
    AddExit("north", MY_ROOM + "/" +"mstreet5");
    AddExit("out", MY_ROOM + "/" +"mstreet5");
    SetDoor("north", "/domains/fort/doors/tanner.c");
}
void init() {
    ::init();
    SetSmell(([  "default" : "The thick, nauseating smell of curing leather hangs heavy here."]));
}
