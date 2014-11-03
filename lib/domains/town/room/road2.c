#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("West Village road");
    SetNightLong("This is a well-traveled road, illuminated by a lamp light. It leads east into town and west toward an old, humpbacked bridge. A grand mansion is south.");
    SetDayLong("This is a well-traveled road, leading east into town and west away from it. An old, humpbacked bridge is west of here. A grand mansion is south.");
    SetItems(([
                ({ "fort", "fortress", "fortress in the distance" }) : "It can't be seen well from here, but far north is what appears to be a large fortress built on a high plateau.",
                ({ "road", "roads" }) : "This is a simple east-west road that goes east into town and west away from it. Another road, paved with cobblestones, intersects here to the north and leads high toward a fortress in the distance.",
                "cobblestone road" : "This is where a cobblestone road begins that is built on an steep incline and rises up as it leads north to a high plateau.",
                ]));
    SetSkyDomain("town");
    SetExits( ([
                "east" : "/domains/town/room/road1",
                "west" : "/domains/town/room/bridge",
                "south" : "/domains/town/room/gate.c",
                ]) );
    AddTerrainType(T_ROAD);
    AddItem(new("/domains/town/obj/lamp"));
}

void init(){
    ::init();
    if(mud_name() == "Dead Souls Omega" && !GetExit("north")){
    }
}
