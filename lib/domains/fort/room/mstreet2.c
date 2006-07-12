/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 15:14:06 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("The Fortress");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(25);
    SetShort("Market Street East");
    SetDayLong("This is the eastern part of Market Street. This street is lined along the south by shops and homes, and at its very westernmost end is the market square. The smithy is north of here and south of here is the cobbler's shop. Market Street continues east and west.");
    SetItems(([
	({ "smithy", "blacksmith" }) : "The smithy is north of here.",
	({ "smith's", "smith", "blacksmith's", "blacksmith's shop" }) : "The blacksmith's shop is north of here.",
	({ "lamp", "lamps", "street lamp", "street lamps" }) : "Oil lamps festoon the walls of buildings here, providing ample illumination at night.",
	({ "fort", "fortress", "wall", "walls" }) : "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security.",
	({ "stones", "stone", "cobblestones", "cobblestone", "pavement", "paving stones" }) : "These cobblestones make up the paved ground here, and are worn smooth with decades, perhaps centuries, of wear.",
	({ "cobbler", "cobbler's", "cobbler's shop", "shop" }) : "The cobbler's shop is south of here.",
	({ "road", "street" }) : "This is a paved street inside the Fortress on the Frontiers.",
      ]));
    SetEnters( ([ 
      ]) );
    SetExits( ([
	"east" : "/domains/fort/room/mstreet",
	"south" : "/domains/fort/room/cobbler",
	"west" : "/domains/fort/room/mstreet3",
	"north" : "/domains/fort/room/smithy.c",
      ]) );
    SetNightLong("This is the eastern part of Market Street. Street lamps illuminate this area almost as brightly as daylight. This street is lined along the south by shops and homes, and at its very westernmost end is the market square. The smithy is north of here and south of here is the cobbler's shop. Market Street continues east and west.");
    SetInventory(([]));

    SetDoor("south", "/domains/fort/doors/cobbler.c");

    SetDoor("north", "/domains/fort/doors/blacksmith.c");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
