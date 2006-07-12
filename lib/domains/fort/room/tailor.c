/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 15:20:06 2005
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
    SetShort("tailor");
    SetLong("The tailor's shop.");
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
    AddExit("out", MY_ROOM + "/" +"mstreet4");
    AddExit("north", MY_ROOM + "/" +"mstreet4");



    SetDoor("north", "/domains/fort/doors/tailor.c");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
