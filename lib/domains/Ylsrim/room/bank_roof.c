/*    /domains/Ylsrim/room/bank_room.c
 *    From the Dead Souls Mud Library
 *    An example of a room with an item that allows jumping and climbing
 *    Created by Descartes of Borg 961010
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("Ylsrim");
    SetClimate("arid");
    SetAmbientLight(30);
    SetShort("The roof of the Bank of Ylsrim");
    SetDayLong("From your perfect vantage point atop the hot adobe of "
            "the Bank of Ylsrim you can see nearly the entire desert "
            "town.  On the south side of the bank, the wall is rough "
            "enough to get a foothold down into Kaliid Road.  The wall "
            "on the other sides of the building are simply too smooth to "
            "climb down. You might be able to jump into the road, as well."); 
    SetNightLong("The lights of Ylsrim look beautiful from this vantage "
            "on the bank roof.");
    AddItem("adobe", "The walls and roof are made of this substance.");
    // this is the wall that can be climbed down
    AddItem(new("/domains/Ylsrim/etc/roof_wall"));
    // this is the road that people can jump into
    AddItem(new("/domains/Ylsrim/etc/road"));
    // this is the roof people can jump from
    AddItem(new("/domains/Ylsrim/etc/roof"));
}
void init(){
    ::init();
}
