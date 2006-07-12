/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Sep 20 02:34:32 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "/domains/fort/customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("The Fortress");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(20);
    SetShort("Warehouse Road South");
    SetDayLong("This is the south end of Warehouse Road. To the east is a door at the base of a tower built into the the fortress wall. The street continues north toward the warehouse and fortress exit. The street also runs west here, toward the fort's village.");
    SetNightLong("This is the south end of Warehouse Road. To the east is a door at the base of a tower built into the the fortress wall. The street continues north into darkness. To the west you can see the flickering lights of the village.");
    SetItems(([  ({"fort", "fortress", "wall", "walls", "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security."}) : "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security." ,
	({"road", "street"}) : "This is a paved street inside the Fortress on the Frontiers." ,
	({"building", "dark building", "large building", "large dark building", "warehouse"}) : "This appears to be some kind of warehouse on the west side of the street. " ,
	({"stones", "stone", "cobblestones", "cobblestone", "pavement", "paving stones"}) : "These cobblestones make up the paved ground here, and are worn smooth with decades, perhaps centuries, of wear." ,
	({"community", "village"}) : "The fortress community is west of here." ,
	"tower" : "This is one of the two towers that flank the fort's entrance. There is a door here, at its base."]));
    SetExits( ([
	"north" : "/domains/fort/room/wroad",
	"west" : "/domains/fort/room/mstreet",
	"east" : "/domains/fort/room/south_gtower_lower.c",
      ]) );
    SetInventory(([]));

    SetDoor("east", "/domains/fort/doors/tower_door1.c");

}
void init() {
    ::init();
}