/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 15:09:43 2005
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
    SetShort("Market Street");
    SetDayLong("This is the central part of Market Street. This street is lined along the south by shops and homes, and at its very westernmost end is the market square. There is a narrow alley north of here and south is the language school. Market Street continues east and west.");
    SetNightLong("This is the central part of Market Street. Street lamps illuminate this area almost as brightly as daylight. This street is lined along the south by shops and homes. There is a narrow alley north of here and south is the language school. Market Street continues east and west.");
    SetItems(([  ({"school", "language school"}) : "This humble building is where locals come to be schooled in languages." ,
	({"fort", "fortress", "wall", "walls"}) : "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security." ,
	({"road", "street"}) : "This is a paved street inside the Fortress on the Frontiers." ,
	({"lamp", "lamps", "street lamp", "street lamps"}) : "Oil lamps festoon the walls of buildings here, providing ample illumination at night." ,
	({"stones", "stone", "cobblestones", "cobblestone", "pavement", "paving stones"}) : "These cobblestones make up the paved ground here, and are worn smooth with decades, perhaps centuries, of wear." ,
	({"alley", "narrow alley"}) : "A narrow alley, leading north." ,
	"q" : ""]));
    SetExits( ([
	"west" : "/domains/fort/room/mstreet4",
	"north" : "/domains/fort/room/alley",
	"east" : "/domains/fort/room/mstreet2",
	"south" : "/domains/fort/room/language.c",
      ]) );
    SetEnters( ([ 
      ]) );
    SetInventory(([]));

    SetDoor("south", "/domains/fort/doors/school.c");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}