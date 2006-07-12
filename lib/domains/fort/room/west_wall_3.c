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
    SetShort("West wall");
    SetDayLong("You are at the top of the west wall. The fortress militia patrol here to surveil the vast western forest, but it's obvious that no serious attack could be made on the fortress from this side. The thirty foot walls and the fifty foot dropoff from the plateau would make any siege impossible. The west wall runs north and south from here.");
    SetNightLong("You are at the top of the west wall. The fortress militia patrol here to surveil the vast western forest. The battlements run north and south from here into darkness.");
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
	"south" : "/domains/fort/room/west_wall_2",
	"north" : "/domains/fort/room/west_wall_4",
      ]));

    SetInventory(([]));

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}