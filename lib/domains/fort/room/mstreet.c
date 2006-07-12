/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 02:08:55 2005
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
    SetDayLong("This is the east end of Market Street. This street is lined along the south by shops and homes, and at its very westernmost end is the market square. A tall building is north of here, but its entry is a bit further west. South of here is the fishmonger's shop. To the east is Warehouse Road, and Market Street continues west. ");
    SetItems(([
	({ "fort", "fortress", "wall", "walls" }) : "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security.",
	({ "road", "street" }) : "This is a paved street inside the Fortress on the Frontiers.",
	({ "lamp", "lamps", "street lamp", "street lamps" }) : "Oil lamps festoon the walls of buildings here, providing ample illumination at night.",
	({ "stones", "stone", "cobblestones", "cobblestone", "pavement", "paving stones" }) : "These cobblestones make up the paved ground here, and are worn smooth with decades, perhaps centuries, of wear.",
	({ "shop", "fishmonger", "fishmonger shop" }) : "The fishmonger is south of here.",
	({ "building", "tall building", "smithy" }) : "This appears to be the smithy, or blacksmith's shop.",
      ]));
    SetInventory(([
	"/domains/fort/npc/cat" : 1,
	"/domains/fort/npc/provost_guard" : 1,
      ]));
    SetEnters( ([ 
      ]) );
    SetExits( ([
	"east" : "/domains/fort/room/croad",
	"west" : "/domains/fort/room/mstreet2",
	"south" : "/domains/fort/room/fishmonger.c",
      ]) );
    SetNightLong("This is the east end of Market Street. Street lamps illuminate this area almost as brightly as daylight. This street is lined along the south by shops and homes. A tall building is north of here, but its entry is a bit further west. South of here is the fishmonger's shop. To the east is Warehouse Road, and Market Street continues west.");

    SetDoor("south", "/domains/fort/doors/fishmonger.c");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
