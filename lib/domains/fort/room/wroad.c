/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Sep 20 02:17:03 2005
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
    SetShort("Warehouse Road");
    SetDayLong("This is a north-south road inside the fortress, bordered on the west by a warehouse building and the east by the entrance gate courtyard. North of here the road runs by the stables. The road continues south, further into town.");
    SetNightLong("This is a north-south road inside the fortress, bordered on the west by a large, dark building and the east by the entrance gate courtyard. The road continues north and south into darkness.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([  ({"fort", "fortress", "wall", "walls", "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security."}) : "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security." ,
	({"road", "street"}) : "This is a paved street inside the Fortress on the Frontiers." ,
	({"stable", "stables"}) : "They aren't quite visible from here." ,
	({"building", "dark building", "large building", "large dark building", "warehouse"}) : "This appears to be some kind of warehouse on the west side of the street. " ,
	({"stones", "stone", "cobblestones", "cobblestone", "pavement", "paving stones"}) : "These cobblestones make up the paved ground here, and are worn smooth with decades, perhaps centuries, of wear." ,
	({"community", "village"}) : "The fortress community is west of here."]));
    SetInventory(([]));
    SetObviousExits("n, s, e");
    AddExit("north", MY_ROOM + "/" +"sroad.c");
    AddExit("east", MY_ROOM + "/" +"courtyard.c");
    AddExit("south", MY_ROOM + "/" +"croad.c");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}