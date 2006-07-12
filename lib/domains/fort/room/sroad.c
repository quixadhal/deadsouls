/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Mon Oct 17 18:18:49 2005
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
    SetNightLight(15);
    SetShort("Warehouse Road end");
    SetDayLong("This is the northern, dead end of a north-south road inside the fortress. West is the entrance to the stables, and the road runs south. ");
    SetNightLong("This is the northern, dead end of a north-south road inside the fortress. West is the entrance to a dark building, and the road runs south into the night. ");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([    ({"fort","fortress","wall","walls"}) : "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security." ,
	({"road","street"}) : "This is a paved street inside the Fortress on the Frontiers." ,
	({"stable","stables"}) : "This building provides shelter for mounts." ,
	({"building","dark building","large building","large dark building"}) : "This appears to be stables on the west side of the street. " ,
	({"stones","stone","cobblestones","cobblestone","pavement","paving stones"}) : "These cobblestones make up the paved ground here, and are worn smooth with decades, perhaps centuries, of wear." ,
	({"community","village"}) : "The fortress community is south and west of here."]));
    SetInventory(([]));
    SetObviousExits("s, w");
    AddExit("south", MY_ROOM + "/" +"wroad");
    AddExit("west", MY_ROOM + "/" +"stables");
    AddEnter("stables", MY_ROOM + "/" +"stables");
    AddEnter("building", MY_ROOM + "/" +"stables");
    AddEnter("stable", MY_ROOM + "/" +"stables");
    AddEnter("dark building", MY_ROOM + "/" +"stables");
    
    
}
void init() {
    ::init();
    SetSmell(([  "default" : "The faint smell of manure lingers here."]));
    SetListen(([]));
}