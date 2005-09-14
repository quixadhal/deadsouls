/*    /domains/Ylsrim/room/kaliid7.c
 *    From the Dead Souls V Object Library
 *    An example simple room with an item in it
 *    Created by Descartes of Borg 970101
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("Ylsrim");
    SetClimate("arid");
    SetNightLight(18);
    SetShort("the western edge of Ylsrim");
    SetLong("Kaliid Road comes to its western end here as Ylsrim disappears "
	    "into the desert.  Not too far east of here you see where "
	    "the local bank rests.");
    AddItem(({ "bank", "bank of ylsrim" }), "Ylsrim's local bank.  It has "
            "an adobe wall.", ({ "ylsrim" }));
    AddItem("desert", "A great desert that surrounds Ylsrim and makes it "
	    "look so fragile.");
    AddItem("road", "The main street in Ylsrim.", "kaliid");
    SetInventory(([ "/domains/Ylsrim/weapon/stick" : 1 ]));
    SetObviousExits("e, w");
    SetExits( ([ "east" : "/domains/Ylsrim/room/"+ "kaliid6",
		"west" : "/domains/Ylsrim/room/"+ "sand_room" ]));
}
void init(){
::init();
}
