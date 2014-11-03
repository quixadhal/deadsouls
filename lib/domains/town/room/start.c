#include <terrain_types.h>
#include <lib.h>
inherit LIB_ROOM;

int readSign() {
    this_player()->more("/domains/town/txt/hints_sign.txt");
    return 1;
}

static void create() {
    room::create();
    SetClimate("outdoors");
    SetAmbientLight(30);
    SetShort("Village Road Intersection");
    SetNightLong("You are in the main intersection of the village, lit by a town lamp post. Saquivor road extends north and south, intersected east to west by a road that leads west toward a wilderness, and east toward shore.\n%^GREEN%^There is a sign here you can read.%^RESET%^");
    SetDayLong("You are in the main intersection of the village. Saquivor road extends north and south, intersected east to west by a road that leads west toward a wilderness, and east toward shore.\n%^GREEN%^There is a sign here you can read.%^RESET%^");
    SetItems( ([
                "sign":"A large sign on the road. To read it, 'read sign'.",
                ]) );
    SetSkyDomain("town");
    SetExits( ([
                "south" : "/domains/town/room/south_road1",
                "east" : "/domains/town/room/vill_road2",
                "west" : "/domains/town/room/road1",
                "north" : "/domains/town/room/road0.c",
                ]) );
    AddTerrainType(T_ROAD);
    SetNoModify(0);
    SetInventory(([
                "/domains/town/obj/clocktower" : 1,
                ]));
    SetEnters( ([
                ]) );
    SetRead("sign", (: readSign :) );
    AddItem(new("/domains/town/obj/lamp"));
}

void init(){
    ::init();
}
