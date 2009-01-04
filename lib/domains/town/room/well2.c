#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(0);
    SetShort("Water Tunnel");
    SetLong("This dark tunnel appears roughly carved into the bedrock, and slopes downward to the west. It seems as though this is the way for an underground spring to feed the town well. The tunnel ends in doors at both the east and west.");
    SetItems(([
                ({ "rock", "bedrock" }) : "This is dense, hard stuff deep underground.",
                ]));
    SetExits( ([
                "east" : "/domains/town/room/well1",
                "west" : "/domains/town/room/well3.c",
                ]) );
    SetDoor("east", "/domains/town/doors/welldoor1.c");
    SetDoor("west", "/domains/town/doors/welldoor2.c");
    SetFlowLimit(2);
}

void init(){
    ::init();
}
