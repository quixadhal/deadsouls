#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;
static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Saquivor Road");
    SetLong("You are on a well-traveled road leading north-south "
      " through the village.  To the east is the entrance to the " 
      "Village Pub, a time-honored traditional gathering place for "
      "adventurers of every race and creed.  To the west is "
      "The Healers Guild. Far to the north, through the clouds, "
      "you see a mountain range; to the south is a major intersection.");
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
    AddTerrainType(T_ROAD);
    SetExits( ([ 
        "east" : "/domains/town/room/tavern",
        "south" : "/domains/town/room/vill_road1",
        "west" : "/domains/town/room/healer",
        "north" : "/domains/town/room/mountain_road",
      ]) );
    SetInventory(([
        "/domains/town/weap/knife" :1,
        "/domains/town/npc/beggar" :1
      ]) );

    SetEnters( ([
        "pub" : "/domains/town/room/tavern",
        "guild" : "/domains/town/room/healer",
        "healers guild" : "/domains/town/room/healer"
      ]) );
    SetNoModify(0);
    SetFlyRoom("/domains/town/virtual/sky/28,1,1");
}

void init(){
    ::init();
}
