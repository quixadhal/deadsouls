/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 15:18:51 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(25);
    SetShort("Market Street");
    SetDayLong("This is the central part of Market Street. This street is lined along the south by shops and homes, and at its very westernmost end is the market square. There is a general store north of here and south is the tailor shop. Market Street continues east and west.");
    SetNightLong("This is the central part of Market Street. Street lamps illuminate this area almost as brightly as daylight. This street is lined along the south by shops and homes. There is a general store north of here and south is the tailor shop. Market Street continues east and west.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([  ({"shop","quartermaster", "quartermaster's", "quartermaster's shop"}) : "The quartermaster's shop lies north of here." ,
	({"tailor", "tailor's shop", "tailor's"}) : "The tailor's shop lies south of here." ,
	({"fort", "fortress", "wall", "walls"}) : "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security." ,
	({"road", "street"}) : "This is a paved street inside the Fortress on the Frontiers." ,
	({"lamp", "lamps", "street lamp", "street lamps"}) : "Oil lamps festoon the walls of buildings here, providing ample illumination at night." ,
	({"stones", "stone", "cobblestones", "cobblestone", "pavement", "paving stones"}) : "These cobblestones make up the paved ground here, and are worn smooth with decades, perhaps centuries, of wear."]));
    SetInventory(([]));
    SetObviousExits("n, s, e, w");
    AddExit("east", MY_ROOM + "/" +"mstreet3");
    AddExit("south", MY_ROOM + "/" +"tailor");
    AddExit("west", MY_ROOM + "/" +"mstreet5");
    AddExit("north", MY_ROOM + "/" +"quartermaster");
    AddEnter("shop", MY_ROOM + "/" +"quartermaster");
    AddEnter("quartermaster's", MY_ROOM + "/" +"quartermaster");
    AddEnter("tailor's shop", MY_ROOM + "/" +"tailor");
    AddEnter("tailor", MY_ROOM + "/" +"tailor");
    AddEnter("quartermaster's shop", MY_ROOM + "/" +"quartermaster");
    AddEnter("quartermaster", MY_ROOM + "/" +"quartermaster");
    AddEnter("tailor's", MY_ROOM + "/" +"tailor");


    SetDoor("south", "/domains/fort/doors/tailor.c");

    SetDoor("north", "/domains/fort/doors/quartermaster.c");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
