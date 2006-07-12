/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 15:26:14 2005
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
    SetShort("Market Street West");
    SetDayLong("This is the western part of Market Street. This street is lined along the south by shops and homes, and at its very westernmost end is the market square. There is a warehouse north of here and south is the tanner's shop. Market Street continues east and west.");
    SetNightLong("This is the western part of Market Street. Street lamps illuminate this area almost as brightly as daylight. This street is lined along the south by shops and homes. There is a warehouse north of here and south is the tanner's shop. Market Street continues east and west.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([    ({"warehouse", "quartermaster's warehouse"}) : "The quartermaster's warehouse lies north of here. Apparently there is no entry to it here: it must be accessed from the quartermaster's shop." ,
	({"tailor", "tailor's shop", "tailor's"}) : "The tailor's shop lies south of here." ,
	({"fort", "fortress", "wall", "walls"}) : "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security." ,
	({"road", "street"}) : "This is a paved street inside the Fortress on the Frontiers." ,
	({"lamp", "lamps", "street lamp", "street lamps"}) : "Oil lamps festoon the walls of buildings here, providing ample illumination at night." ,
	({"stones", "stone", "cobblestones", "cobblestone", "pavement", "paving stones"}) : "These cobblestones make up the paved ground here, and are worn smooth with decades, perhaps centuries, of wear." ,
	({"tanner", "tanner's", "tanner's shop", "shop"}) : "The tanner's shop is south of here."]));
    SetInventory(([]));
    SetObviousExits("s, e, w");
    AddExit("east", MY_ROOM + "/" +"mstreet4");
    AddExit("west", MY_ROOM + "/" +"mstreet6");
    AddExit("south", MY_ROOM + "/" +"tanner");
    AddEnter("tanner's", MY_ROOM + "/" +"tanner");
    AddEnter("tanner's shop", MY_ROOM + "/" +"tanner");
    AddEnter("tanner", MY_ROOM + "/" +"tanner");
    AddEnter("shop", MY_ROOM + "/" +"tanner");
    SetDoor("south", "/domains/fort/doors/tanner.c");

}
void init() {
    ::init();
}