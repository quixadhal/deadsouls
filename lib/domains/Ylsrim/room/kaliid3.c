/*    /domains/Ylsrim/room/kaliid3.c
 *    From the Dead Souls V Object Library
 *    An example of a room with a door and press event handler
 *    Created by Descartes of Borg 961222
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("Ylsrim");
    SetClimate("arid");
    SetNightLight(18);
    SetShort("Kaliid Road by the Church");
    SetLong("Kaliid Road moves east and west through the desert town of "
	    "Ylsrim.  The road itself gets sandier as it gets swallowed by "
	    "the desert in the distance east.  On the northern end of the "
	    "road a door belonging to the local church.  Next to the door "
	    "sits a huge button.");
    AddItem("church", "It is a rather pathetic wooden building where the "
	    "local priests initiate people into their religion.");
    AddItem("desert", "The town does all it can to keep from being swallowed "
	    "by it.");
    AddItem(new("/domains/Ylsrim/etc/church_button"));
    SetObviousExits("w, enter church");
    SetExits( ([ 
	       "west" : "/domains/Ylsrim/room/"+ "kaliid4" ]));
SetEnters( ([ "church" : "/domains/Ylsrim/room/"+ "church" ]));
    SetDoor("church", "/domains/Ylsrim/etc/church_door");
}

void init(){
::init();
}
