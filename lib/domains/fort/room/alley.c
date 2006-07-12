/* 
File created with Roommaker 
written by Haderach@Frontiers 
Room made by Cratylus
Date: Tue Oct 18 03:52:12 2005
WARNING! WARNING! If you edit this room by hand the room maker may be unable to read
it properly. Hand-edit at your own risk.

*/

#include <lib.h>
#include "/domains/fort/customdefs.h"
inherit LIB_ROOM;

static void create() {
    room::create();
    SetTown("The Fortress");
    SetClimate("temperate");
    SetDayLight(30);
    SetNightLight(25);
    SetShort("narrow alley");
    SetDayLong("This narrow alley connects north with Guild Street and South with market street. The building forming the west wall is the Quartermaster's shop. The building to the east is the smithy.");
    SetNightLong("This narrow alley runs north and south, connecting two streets. Large buildings form the east and west walls.");
    SetItems(([]));
    SetInventory(([]));
    AddExit("north", MY_ROOM + "/" +"gstreet2");
    AddExit("south", MY_ROOM + "/" +"mstreet3");

}
void init() {
    ::init();
    SetSmell(([]));
    SetListen(([]));
}