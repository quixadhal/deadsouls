/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 14:34:36 2005
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
    SetShort("prison tower interior");
    SetLong("inside the prison tower");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([]));
    SetObviousExits("n, s, e, w, u, d");
    AddExit("south", MY_ROOM + "/" +"cell3");
    AddExit("north", MY_ROOM + "/" +"cell1");
    AddExit("up", MY_ROOM + "/" +"ptower_top");
    AddExit("down", MY_ROOM + "/" +"bastion");
    AddExit("west", MY_ROOM + "/" +"cell4");
    AddExit("east", MY_ROOM + "/" +"cell2");
    //funs
    //snuf
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
//extras
