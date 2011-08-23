#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("North Saquivor Road");
    SetNightLong("Illuminated by lamplight here is Saquivor road, leading north-south through the village. To the east is the village pub, the legendary watering hole used by thousands of rogues, paladins, and everyone in between. The Healers' Guild is west. To the south is a major intersection. The road continues north into the gloom.");
    SetDayLong("You are on Saquivor road, leading north-south through the village. To the east is the village pub, the legendary watering hole used by thousands of rogues, paladins, and everyone in between. The Healers' Guild is west, and to the north you can see a road leading up to the plateau where the Fortress on the Frontiers stands. To the south is a major intersection.");
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
        "/domains/town/weap/knife" : 1,
      ]));
    SetExits( ([
        "north" : "/domains/town/room/mountain_road",
        "east" : "/domains/town/room/tavern",
        "west" : "/domains/town/room/healer",
        "south" : "/domains/town/room/road0.c",
      ]) );
    AddTerrainType(T_ROAD);

    SetEnters( ([
        "pub" : "/domains/town/room/tavern",
        "guild" : "/domains/town/room/healer",
        "healers guild" : "/domains/town/room/healer"
      ]) );
    SetNoModify(0);
    AddItem(new("/domains/town/obj/lamp"));
}

void init(){
    ::init();
}
