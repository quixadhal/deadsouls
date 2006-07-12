/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Mon Oct 17 18:57:42 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("The Fortress");
    SetClimate("indoors");
    SetDayLight(30);
    SetNightLight(15);
    SetShort("south gate tower, lower level");
    SetDayLong("You are inside the south gate tower, at the bottom. Stairs lead up to the top level, where the fortress militia patrol the battlements to guard against invaders. Here, inside the base, there is nothing much to see other than the thick interior walls of the base of the tower. Through the west exit is Market Street.");
    SetNightLong("You are inside the south gate tower, at the bottom. Stairs lead up to the top level, where the fortress militia patrol the battlements to guard against invaders. Here, inside the base, there is nothing much to see other than the thick interior walls of the base of the tower. Through the west exit is Market Street.");
    SetItems(([  "base" : "You are at the base of the tower." ,
	"top" : "You cannot quite see the top of the tower from here." ,
	({"wall", "walls", "thick interior walls", "thick interior wall", "interior wall", "interior walls", "thick walls", "thick wall", "tower"}) : "These walls appear extremely strong and thick, as one would expect of a military fortification." ,
	({"stairs", "stair", "stairway", "stairwell"}) : "This is a set of stairs that circles the interior wall of the tower, leading up."]));
    SetExits( ([
	"up" : "/domains/fort/room/south_gtower_top",
	"stairs" : "/domains/fort/room/south_gtower_top",
	"west" : "/domains/fort/room/croad.c",
      ]) );
    SetInventory(([]));

    SetDoor("west", "/domains/fort/doors/tower_door1.c");

}
void init() {
    ::init();
}
