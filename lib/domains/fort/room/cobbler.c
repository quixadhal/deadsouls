/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 02:35:33 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "/domains/fort/customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("The Fortress");
    SetClimate("indoors");
    SetAmbientLight(30);
    SetNightLight(0);
    SetDayLight(25);
    SetShort("cobbler's shop");
    SetLong("This barren place was once the bustling workshop and store of Iebedo the cobbler. It appears that the place is deserted, and nothing remains of that previous time except the bare walls.");
    SetItems(([
	({ "wall", "walls", "bare wall", "bare walls" }) : "Walls with nothing on them.",
      ]));
    SetExits( ([ 
	"north" : "/domains/fort/room/mstreet2",
      ]) );
    SetInventory(([]));

    SetDoor("north", "/domains/fort/doors/cobbler.c");

}
void init() {
    ::init();
    SetListen(([]));
}