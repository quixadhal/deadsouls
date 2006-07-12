/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Sep 20 02:04:19 2005
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
    SetNightLight(20);
    SetShort("courtyard");
    SetDayLong("Sunlight streams down on this courtyard, warming the cobblestones that pave the ground. This courtyard is the first place visitors see inside the fortress, and many are surprised to discover that they are still outdoors. The mighty walls of the fort are there to protect this little community, which has shops, bankers, farmers, and guilds just like any other village. The outside world is east of here, and the rest of the village is west.");
    SetNightLong("Moonlight barely illuminates this courtyard, which serves as the first place inside the fort that travelers see. Although it is nighttime, there is still plenty of activity going on. It seems that the community inside the fortress is quite active. The lights of the village beckon west. The wilderness outside is east.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([  ({"fort", "fortress", "wall", "walls", "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security."}) : "This is the interior of the fortress, and this side of the walls inspires a sense of safety and security." ,
	({"stones", "stone", "cobblestones", "cobblestone", "pavement", "paving stones"}) : "These cobblestones make up the paved ground here, and are worn smooth with decades, perhaps centuries, of wear." ,
	({"courtyard", "court yard", "here", "place", "this place"}) : "This is a courtyard inside the fortress, right next to the entry gate." ,
	({"community", "village"}) : "The fortress community is west of here."]));
    SetInventory(([
	"/domains/fort/npc/provost_guard" : 2,
	"/domains/fort/npc/gate_guard" : 1,
	"/domains/fort/obj/bench" : 2,
      ]));
    SetExits( ([ 
	"east" : "/domains/fort/room/gate",
	"west" : "/domains/fort/room/wroad",
      ]) );
    SetDoor("east",MY_AREA + "/doors/gate");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
