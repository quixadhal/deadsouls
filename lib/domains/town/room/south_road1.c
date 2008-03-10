#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Saquivor Road");
    SetLong("This is a cobblestone road, leading "+
      "north into town and south away from it. "+
      "The First Village Bank stands here on "+
      "the east side of the road. The magic "+
      "shop stands west. "+
      "\n%^GREEN%^An old well stands by the "+
      "side of the road.%^RESET%^");
    SetItems( ([
        ({"well","old well","dry well"}) : "This "
        "looks like the sort of system used to "
        "provide water before the town implemented "
        "more modern waterworks. It looks long dry "
        "and abandoned.",
        "bank" : "The First Village Bank",
        ({"road","cobblestone road"}) : "A "
        "cobblestone road running north and south."
      ]) );
    AddTerrainType(T_ROAD);
    SetExits( ([
        "south" : "/domains/town/room/south_road2",
        "north" : "/domains/town/room/vill_road1",
        "east" : "/domains/town/room/bank",
        "west" : "/domains/town/room/magic_shop.c",
      ]) );
    SetNoModify(0);
    SetEnters( ([
        "well" : "/domains/town/room/well1"
      ]) );
    SetFlyRoom("/domains/town/virtual/sky/28,99999,1");
}
void init(){
    room::init();
}
