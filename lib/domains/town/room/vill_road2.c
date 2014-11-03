#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("East Village Road");
    SetNightLong("You are on a long road, lit by a lamp post. The general store is north. The road stretches east and west through the town. The Adventurers' Guild is south.");
    SetDayLong("You are on a long road. The general store is north. The road stretches east and west through the town. The Adventurers' Guild is south.");
    SetItems( ([
                ({"adventurers guild","guild"}) : "This small "
                "building on the southern side of the road is where "
                "adventurers, great and small, come to record "
                "their exploits and seek advancement.",
                ({"shop","store","general store"}) : "This is the "
                "general store, where almost anything can be "
                "bought or sold.",
                ({"road","long road"}) : "An east-west cobblestone "
                "road through town.",
                ]) );
    SetSkyDomain("town");
    AddTerrainType(T_ROAD);
    SetEnters( ([ 
                ]) );
    SetExits( ([
                "north" : "/domains/town/room/shop",
                "south" : "/domains/town/room/adv_guild",
                "east" : "/domains/town/room/vill_road3",
                "west" : "/domains/town/room/start",
                ]) );
    AddItem(new("/domains/town/obj/lamp"));
}
void init(){
    ::init();
}
