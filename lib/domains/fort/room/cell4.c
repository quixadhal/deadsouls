/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 14:36:49 2005
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
    SetAmbientLight(30);
    SetShort("prison cell");
    SetLong("prison cell");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([]));
    SetObviousExits("e");
    AddExit("east", MY_ROOM + "/" +"ptower");
    //funs
    //snuf
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
//extras
