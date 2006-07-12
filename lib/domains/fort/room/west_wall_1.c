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
    SetShort("Southwest wall");
    SetDayLong("You are at the top of the southwest wall. This is where the fortress militia patrol to surveil the comings and goings of travelers on Commerce Road, an east to west path south of the fortress. The battlements of this wall run southeast to the Lookout Tower, and continue north toward the west wall.");
    SetNightLong("You are at the top of the southwest wall. The battlements of this wall run southeast and north into the night.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([      ({"wall", "walls"}) : "These walls appear extremely strong and thick, as one would expect of a military fortification." ,
	({"battlement", "battlements"}) : "These are defensive structures along the top of the wall that provide cover for defenders."]));
    SetInventory(([
	"/domains/fort/npc/tower_guard2" : 1,
      ]));
    SetExits(([
	"north" : "/domains/fort/room/west_wall_2",
	"southeast" : "/domains/fort/room/sw_wall",
      ]));

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}