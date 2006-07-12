/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 14:26:16 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "/realms/slymenstra/customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("governor's office");
    SetLong("office of the boss.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([]));
    SetExits( ([ 
	"south" : "/domains/fort/room/ghall",
	"west" : "/domains/fort/room/quarters2",
	"east" : "/domains/fort/room/quarters1",
      ]) );
    SetInventory(([]));

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}