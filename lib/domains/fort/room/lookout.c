/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 14:46:36 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "/domains/fort/customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(25);
    SetShort("Lookout Tower");
    SetDayLong("This is the top of the southwest lookout tower. It overlooks the southern marshes and Commerce Road, a long stretch that runs from the eastern seashore to the west as far as the eye can see. The fortress battlements run east and northwest. The fort's south wall is east, and its southwest wall is northwest.");
    SetNightLong("This is the top of the southwest lookout tower. During the daytime it overwatches the southern marshes and Commerce Road. The fortress battlements run east and northwest into darkness.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([      ({"wall", "walls"}) : "These walls appear extremely strong and thick, as one would expect of a military fortification." ,
	({"battlement", "battlements"}) : "These are defensive structures along the top of the wall that provide cover for defenders."]));
    SetExits(([
	"northwest" : "/domains/fort/room/sw_wall",
	"east" : "/domains/fort/room/south_wall_5",
      ]));

    SetInventory(([]));

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}