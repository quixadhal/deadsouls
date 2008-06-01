#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("West Village road");
    SetDayLong("This is a well-traveled road, leading east into town and west away from it. The town church is north. A grand mansion is south.");
    SetNightLong("This is a well-traveled road, illuminated by a lamp light. It leads east into town and west away from it. The town church is north. A grand mansion is south.");
    AddTerrainType(T_ROAD);
    SetExits( ([
        "north" : "/domains/town/room/church",
        "east" : "/domains/town/room/vill_road1",
        "west" : "/domains/town/room/road2",
        "south" : "/domains/town/room/gate",
      ]) );
    SetFlyRoom("/domains/town/virtual/sky/27,100000,1");
    AddItem(new("/domains/town/obj/lamp"));
}
void init(){
    ::init();
}
