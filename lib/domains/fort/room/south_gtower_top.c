/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 01:06:47 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

int PreExit(){
    object *livings;
    livings = get_livings(this_object());
    foreach(object living in livings){
	if(!answers_to("gate guard", living) && !answers_to("tower guard", living) && !creatorp(this_player())) {
	    write("The guard bars your way, saying \"Security personnel only. Sorry.\"");
	    return 0;
	}
    }
    return 1;
}

#include <lib.h>
#include "/domains/fort/customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(25);
    SetShort("south gate tower");
    SetDayLong("You are at the top of the south gate tower. Stairs lead down to the lower level. This is where the fortress militia patrol the battlements to guard against invaders. The view is remarkable here, not only of the gate immediately below, but of the great northeastern forest. The battlements of the fort's eastern wall run north and south of here.");
    SetItems(([
	"tower" : "This is one of the towers that flank the main gate.",
	({ "battlement", "battlements" }) : "These are defensive structures along the top of the wall that provide cover for defenders.",
	({ "stair", "stairs", "stairwell", "stairway", "steps" }) : "This is a set of stairs that circles the interior wall of the tower, leading down.",
	({ "view", "great northeastern forest", "forest", "northeastern forest", "gate" }) : "The view from up here is simply breathtaking.",
	({ "wall", "walls" }) : "These walls appear extremely strong and thick, as one would expect of a military fortification.",
      ]));
    SetNightLong("You are at the top of the south gate tower. Stairs lead down to the lower level. This is where the fortress militia patrol the battlements to guard against invaders. The night sky does little to illuminate the vast wilderness that must lie northeast of here. The battlements of the fort's eastern wall run north and south of here.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetInventory(([  MY_NPC + "/" +"gate_guard" : 1]));
    SetObviousExits("n, s, d");
    AddExit("down", MY_ROOM + "/" +"south_gtower_lower");
    AddExit("north", MY_ROOM + "/" +"north_gtower_top", (: PreExit :));
    AddExit("south", MY_ROOM + "/" +"east_wall_south", (: PreExit :));
    AddEnter("tower", MY_ROOM + "/" +"south_gtower_lower");

}
void init() {
    ::init();
    SetListen(([]));
}
