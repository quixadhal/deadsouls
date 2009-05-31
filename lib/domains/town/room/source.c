#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("indoors");
    SetAmbientLight(0);
    SetShort("a dark tunnel");
    SetLong("This tunnel has very smooth stone walls and leads east. There is also an opening in the ceiling above.");
    SetItems(([
                ({ "hole", "holes", "floor" }) : "These holes appear to be where spring water enters to flood the chamber and fill the well east of here. ",
                ({ "dirt", "dust" }) : "There's plenty of that here. Empty wells rarely get much priority on cleaning day.",
                ({ "bottom", "well" }) : "Dirty, musty, and unpleasant.",
                ({ "vermin", "rats", "bugs" }) : "Looks like they're hiding from you at the moment.",
                "haven" : "A nice place for vermin.",
                ({ "rock", "bedrock" }) : "This is dense, hard stuff deep underground.",
                ]));
    SetFlowLimit(40);
    SetInventory( ([
                "/domains/town/obj/water_pipe" : 1,
                "/domains/town/obj/waterwheel" : 1,
                ]) );
    SetExits( ([
                "up" : "/domains/town/room/well3",
                "east" : "/domains/town/room/wtunnel2.c",
                ]) );

    SetDoor("up", "/domains/town/doors/grate.c");

}
void init(){
    ::init();
}
