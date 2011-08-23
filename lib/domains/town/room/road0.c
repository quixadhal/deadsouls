#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Saquivor Road");
    SetNightLong("Illuminated by lamplight here is Saquivor road, leading north-south through the village. The road continues north toward some buildings, and south toward an intersection.");
    SetDayLong("This is Saquivor road, leading north-south through the village. A bit further north are what look like a pub and some kind of guild. To the south is a major intersection.");
    SetItems( ([
        ({"pub","tavern","village pub"}) : "The is the town's "
        "legendary watering hole, where refreshments are served "
        "and tall tales are swapped.",
        ({"guild","healers guild"}) : "In this building is "
        "the office of the only doctor in town.",
        "clouds" : "Little puffy clouds surround the "
        "peaks of the northern mountains.",
        ({"northern mountains","mountains","range","mountain range"}): 
        "Majestic mountains are far to the north, seeming "
        "almost purple from here.",
        "intersection" : "The main town crossroads is south."
      ]) );
    SetSkyDomain("town");
    SetInventory(([
        "/domains/town/npc/beggar" : 1,
      ]));
    SetExits( ([
        "north" : "/domains/town/room/road",
        "south" : "/domains/town/room/start.c",
      ]) );

    AddTerrainType(T_ROAD);

    SetNoModify(0);
    AddItem(new("/domains/town/obj/lamp"));
}

void init(){
    ::init();
}
