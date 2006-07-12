/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 02:59:08 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "/domains/fort/customdefs.h"
inherit LIB_ROOM;

object ob;

static void create() {
    room::create();
    SetTown("The Fortress");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetShort("The language school");
    SetLong("This is the famous Language School of the Fortress on the Frontiers. People travel from faraway lands to take lessons from the teacher here, who is legendary for knowing and teaching all possible languages. Market street is north.");
    SetItems(([]));
    SetInventory(([
	"/domains/fort/obj/chair" : 1,
	"/domains/fort/npc/limpseur" : 1,
      ]));
    SetExits( ([ 
	"north" : "/domains/fort/room/mstreet3",
      ]) );

    SetDoor("north", "/domains/fort/doors/school.c");

}
void init() {
    ::init();
    if(ob = present("limpseur")) call_out( (: ob->eventForce("sit on chair") :), 1);
}
