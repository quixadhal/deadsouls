/*    /domains/Ylsrim/room/kaliid4.c
 *    From the Dead Souls V Object Library
 *    A simple example room
 *    Created by Descartes of Borg 950929
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("Ylsrim");
    SetClimate("arid");
    SetNightLight(18);
    SetShort("Kaliid Road north of the bazaar");
    SetLong("Kaliid Road stretches as far as you can see east and west "
	    "across Ylsrim.  South, Kaliid opens up into the central "
	    "bazaar where vendors sell goods day and night.  "
	    "on the opposite side of the road is the entrance to "
	    "the local adventurers' hall.");
    AddItem("hall", "A small building which serves as the focal point for "
	    "the adventuring activities for which Ylsrim's support "
	    "is widely known.", ({ "adventurer", "adventurers" }));
    AddItem("bazaar",  "The central bazaar is the main marketplace of "
	    "Ylsrim.  Though many of the shops close at night, vendors "
	    "wander the bazaar at all times pawning their goods.",
	    ({ "central" }));
    AddItem("vendor", "Thre are many vendors wandering about the bazaar.");
    AddItem("road", "The main street for Ylsrim.", ({ "kaliid" }));
    SetObviousExits("e, w, s, enter hall");
    SetExits( ([ "east" : "/domains/Ylsrim/room/"+ "kaliid3",
	       "west" : "/domains/Ylsrim/room/"+ "kaliid5",
	       "south" : "/domains/Ylsrim/room/"+ "bazaar" ]) );
    SetEnters( ([ "hall" : "/domains/Ylsrim/room/"+ "adv_hall" ]));
}

void init(){
::init();
}

