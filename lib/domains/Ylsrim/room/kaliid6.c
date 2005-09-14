/*    /domains/Ylsrim/room/kaliid6.c
 *    From the Dead Souls V Object Library
 *    An example of a room with an object that allows climbing
 *    Created by Descartes of Borg 961010
 */

#include <lib.h>
#include <climb.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("Ylsrim");
    SetClimate("arid");
    SetNightLight(18);
    SetShort("Kaliid Road south of the bank");
    SetLong("Kaliid Road stretches as far as you can see east and west "
	    "across Ylsrim.  The rough wall on the north side of the road "
	    "looks like the distinctive adobe of the Bank of Ylsrim.  As "
	    "the road travels along side the bank walls west, you can see "
	    "Kaliid Road intersect with another.");
    AddItem(({ "bank", "bank of ylsrim" }), "Ylsrim's local bank.  It has "
            "an adobe wall filled with holes.", ({ "ylsrim" }));
    AddItem("road", "The main street in Ylsrim.", "kaliid");
    AddItem("holes", "You might be able to use them to climb the wall.");
    // here is the thing that can be climbed, have a look at it!
    AddItem(new("/domains/Ylsrim/etc/wall"));
    SetObviousExits("e, w");
    SetExits( ([ "east" : "/domains/Ylsrim/room/"+ "kaliid5",
		"west" : "/domains/Ylsrim/room/"+ "kaliid7" ]));
}
void init(){
::init();
}
