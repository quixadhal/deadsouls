#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("West Village road");
    SetDayLong("This is a well-traveled road, leading east into town and west away from it. The town church is north.");
    SetSkyDomain("town");
    SetExits( ([ 
        "north" : "/domains/town/room/church",
        "east" : "/domains/town/room/start",
        "west" : "/domains/town/room/road2",
      ]) );
    SetNightLong("This is a well-traveled road, illuminated by a lamp light. It leads east into town and west away from it. The town church is north.");
    AddTerrainType(T_ROAD);
    AddItem(new("/domains/town/obj/lamp"));
}
void init(){
    ::init();
}
