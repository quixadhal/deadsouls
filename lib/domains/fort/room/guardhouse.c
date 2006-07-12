/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 00:48:57 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "../customdefs.h";
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("fort");
    SetClimate("indoors");
    SetAmbientLight(0);
    SetShort("watchtower guardhouse");
    SetLong("You are in a room at the base of the main watchtower. This guardhouse provides the fortress guards a place to rest and shelter from the weather during breaks. It is also a convenient location to store some weapons and armor in case of emergency.");
    SetProperties (([
	"no attack":0, 
	"no bump":0, 
	"no steal":0, 
	"no magic":0, 
	"no paralyze":0, 
	"no teleport":0]));
    SetItems(([            ({"stair", "stairs", "stairway", "stairwell", "steps"}) : "This is a set of stairs that circles the interior wall of the tower, leading up."]));
    SetInventory(([  MY_AREA + "/" +"obj/gchest" : 1 ,


	MY_OBJ + "/" +"table" : 1 ,

	MY_OBJ + "/" +"gstool" : 1 ,

	MY_OBJ + "/" +"wrack" : 1 ,
	MY_OBJ + "/" +"gchair" : 1]));
    SetObviousExits("u");
    AddExit("up", MY_ROOM + "/" +"watchtower");
    AddExit("stairs", MY_ROOM + "/" +"watchtower");
    //funs
    //snuf
}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}
//extras
