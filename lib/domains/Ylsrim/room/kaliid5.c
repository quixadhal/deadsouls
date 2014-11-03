/*    /domains/Ylsrim/room/kaliid5.c
 *    From the Dead Souls Mud Library
 *    A simple room 
 *    Created by Descartes of Borg 960512
 */

#include <lib.h>

inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("Ylsrim");
    SetClimate("arid");
    SetAmbientLight(30);
    SetShort("Kaliid Road near the Fighters' Hall");
    SetLong("Kaliid Road stretches as far as you can see east and west "
            "across Ylsrim.  Along the north side of this stretch of road "
            "sits the local Fighters' Hall.  Immediately inside the entrance "
            "you notice a recruitment area where novice seekers of fortune "
            "decide to make their ways as fighters.");
    SetSkyDomain("town");
    AddItem("hall", "Fighters passing through Ylsrim stop in to yap about "
            "their adventures, and novices sign up to become fighters.",
            ({ "fighter", "fighters" }));
    AddItem("road", "The main street for Ylsrim.", ({ "kaliid" }));
    SetExits( ([ "east" : "/domains/Ylsrim/room/"+ "kaliid4",
                "west" : "/domains/Ylsrim/room/"+ "kaliid6",
                "north" : "/domains/Ylsrim/room/"+ "fighter_hall" ]));
}
void init(){
    ::init();
}
