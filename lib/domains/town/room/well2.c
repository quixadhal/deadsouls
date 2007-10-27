#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(0);
    SetShort("Water Tunnel");
    SetLong("This dark tunnel appears roughly carved into the bedrock, and slopes downward to the west. It seems as though this may have been a way for an underground spring to feed the town well. The tunnel ends in doors at both the east and west.");
    SetItems(([
        ({ "bottom", "well" }) : "Dirty, musty, and unpleasant.",
        ({ "vermin", "rats", "bugs" }) : "Looks like they're hiding from you at the moment.",
        ({ "dirt", "dust" }) : "There's plenty of that here. Empty wells rarely get much priority on cleaning day.",
        ({ "rock", "bedrock" }) : "This is dense, hard stuff deep underground.",
        "haven" : "A nice place for vermin.",
      ]));
    SetExits( ([
        "east" : "/domains/town/room/well1",
        "west" : "/domains/town/room/well3.c",
      ]) );

    SetDoor("east", "/domains/town/doors/welldoor1.c");

    SetDoor("west", "/domains/town/doors/welldoor2.c");

}
void init(){
    ::init();
}
