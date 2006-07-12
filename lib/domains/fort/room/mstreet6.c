/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Wed Oct 19 02:37:34 2005
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
    SetDayLong("This is the western end of Market Street. This street is\nlined by shops and homes. To the west is Market Square. There is a private residence north of here and south is the locksmith. Market Street continues east. ");
    SetNightLong("This is the western end of Market Street. Street lamps illuminate this area almost as brightly as daylight. There is a private residence north of here and south is the locksmith. To the west is Market Square. There is a  Market Street continues east.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([  "locksmith" : "The fort's locksmith is south." ,
	({"fort", "fortress", "wall", "walls"}) : "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security." ,
	({"road", "street"}) : "This is a paved street inside the Fortress on the Frontiers." ,
	({"lamp", "lamps", "street lamp", "street lamps"}) : "Oil lamps festoon the walls of buildings here, providing ample illumination at night." ,
	({"stones", "stone", "cobblestones", "cobblestone", "pavement", "paving stones"}) : "These cobblestones make up the paved ground here, and are worn smooth with decades, perhaps centuries, of wear." ,
	({"home", "house", "residence"}) : "To the south is someone's home."]));
    SetInventory(([]));
    SetObviousExits("n, s, e, w");
    AddExit("east", MY_ROOM + "/" +"mstreet5");
    AddExit("south", MY_ROOM + "/" +"locksmith");
    AddExit("north", MY_ROOM + "/" +"dairy");
    AddExit("west", MY_ROOM + "/" +"msquare");
    AddEnter("locksmith", MY_ROOM + "/" +"locksmith");
    SetDoor("south", "/domains/fort/doors/locksmith.c");

    SetDoor("north", "/domains/fort/doors/dairy.c");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}