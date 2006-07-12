/* 
   File created with Roommaker 
   written by Haderach@Frontiers 
   Room made by Cratylus
Date: Wed Oct 19 19:50:30 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h"
inherit LIB_ROOM;

string extraLong(string str);

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(20);
    SetShort("The Great Gate");
    SetLong( (: extraLong :));
    SetExits( ([
	"west" : "/domains/fort/room/courtyard",
	"southeast" : "/domains/fort/room/f_road1.c",
      ]) );
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([    ({"portcullis", "bars", "portcullis bars"}) : "This is an entry control system which consists of thick metal bars that can be raised or lowered, as security conditions require. At the moment, the bars are raised, permitting free access to the fort." ,
	({"stone", "stones", "cobblestone", "cobblestones"}) : "These are milled, smoothed rocks that are used as paving on a road, to prevent the road from becoming impassable in bad weather and delay environmental erosion of the path." ,
	({"road", "fort road", "path"}) : "This is an old cobblestone paved path which has clearly seen many years of heavy use. Ruts are worn into the stone where thousands of caravan wagons have traveled in the past. The road ends here at the fortress gate, and leads southeast into wilderness." ,
	({"fort", "fortress", "wall", "walls", "battlement", "battlements", "building", "castle"}) : "This is the famous Fort on the Frontiers. Its exterior consists of huge, thirty-foot high stone walls with battlements on top. The walls stretch about a quarter mile along each side of its roughly square construction.  At each corner and at strategic spots along the perimeter, towers and defensive platforms are built into the walls, from which defenders can lay waste to attackers." ,
	({"towers", "tower", "watchtower", "watchtowers"}) : "These tall towers are built into the fortress walls, and flank the main entrance gate. They soar thirty feet into the sky, and must certainly provide an excellent vantage point for defense."]));
    SetInventory(([  MY_AREA + "/" +"obj/banner" : 1 ,
	MY_NPC + "/" +"gate_guard" : 2]));
    SetDoor("west",MY_AREA + "/doors/gate");


}

string extraLong(string str){
    int closed;
    string extra;
    closed = load_object(MY_AREA + "/doors/gate")->GetClosed();
    if(!closed) extra = "The great gate is open, and the bars raised, permitting free entry west into the fort.";
    else extra = "The great gate is closed and the bars lowered. It would seem that a guard must be asked for access inside.";

    if(!query_night()) return "Fort Road ends here at the great gate of the fortress: a drawbridge flanked by thirty-foot watchtowers. Travel away from here is on Fort Road, leading southeast. " + extra;
    else return "Fort Road ends here at the great gate of the fortress: a drawbridge flanked by thirty-foot watchtowers. Travel away from here is on Fort Road, leading southeast into the night. Lights atop the battlements twinkle through the darkness. " + extra;
}

void init() {
    ::init();
}
