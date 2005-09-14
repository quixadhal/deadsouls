/*    /domains/Ylsrim/room/s_bazaar.c
 *    From the Dead Souls V Object Library
 *    A simple room example with a unique, wandering NPC
 *    Created by Descartes of Borg 960302
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("Ylsrim");
    SetClimate("arid");
    SetNightLight(15);
    SetShort("The south end of the Ylsrim Bazaar");
    SetLong("Along the south end of the Ylsrim Bazaar stands a restaurant "
	    "and a pub.  The central area of the Bazaar is just north of "
	    "here.");
    AddItem("pub", "You can buy yourself an ale from one of Ylsrim's most "
	    "respected residents.", ({ "lars" }));
    AddItem("restaurant", "This restaurant is known for its stew.",
	    ({ "toral" }));
    SetInventory(([ "/domains/Ylsrim/npc/traveller" : -1 ]));
    SetObviousExits("n, enter pub, enter restaurant");
    SetExits( ([ "north" : "/domains/Ylsrim/room/"+ "bazaar" ]) );
    SetEnters( ([ "restaurant" : "/domains/Ylsrim/room/"+ "restaurant",
		"pub" : "/domains/Ylsrim/room/"+ "pub" ]) );
}
void init(){
::init();
}
