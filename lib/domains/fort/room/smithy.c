/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 02:39:02 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("Wulkan's Smithy");
    SetLong("This is the hellishly loud, infernally hot smithy, where Wulkan toils over his forge day and night, fashioning crude metal into fine tools, weapons, and instruments. The forge belches smoke, making it hard to see, and the roar of the bellows, clanking of metal, mind-bending heat make this place intolerable for most people.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetInventory(([
	"/domains/fort/npc/wulkan" : 1,
      ]));
    SetExits( ([ 
	"south" : "/domains/fort/room/mstreet2",
      ]) );

    SetDoor("south", "/domains/fort/doors/blacksmith.c");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
