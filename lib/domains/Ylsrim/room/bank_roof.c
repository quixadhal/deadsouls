/*    /domains/Ylsrim/room/bank_room.c
 *    From the Dead Souls V Object Library
 *    An example of a room with an item that allows jumping and climbing
 *    Created by Descartes of Borg 961010
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("Ylsrim");
    SetClimate("arid");
    SetNightLight(7);
    SetShort("The roof of the Bank of Ylsrim");
    SetDayLong("From your perfect vantage point atop the hot adobe of "
	       "the Bank of Ylsrim you can see nearly the entire desert "
	       "town.  On the south side of the bank, the wall is rough "
	       "enough to get a foothold down into Kaliid Road.  The wall "
	       "on the other sides of the building are simply too smooth to "
	       "climb down.  A hole in the center of the roof opens into "
	       "the bank.");
    SetNightLong("The lights of Ylsrim look beautiful from this vantage "
		 "on the bank roof.  You can barely make out a hole in "
		 "the middle of the roof.");
    AddItem("adobe", "The walls and roof are made of this substance.");
    // this is the wall that can be climbed down
    AddItem(new("/domains/Ylsrim/etc/roof_wall"));
    // this is the road that people can jump into
    AddItem(new("/domains/Ylsrim/etc/road"));
    // this is the roof people can jump from
    AddItem(new("/domains/Ylsrim/etc/roof"));
    // this is the hole people can jump into
    AddItem(new("/domains/Ylsrim/etc/hole"));
}
void init(){
::init();
}
