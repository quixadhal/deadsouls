#include <lib.h>
#include <daemons.h>
#include <medium.h>
#include <terrain_types.h>

inherit LIB_ROOM;

static void create() {
    object ob;
    room::create();
    SetAmbientLight(0);
    SetNoClean(1);
    SetTerrainType(T_SEAFLOOR);
    SetMedium(MEDIUM_WATER);

    SetShort("the sea floor");
    SetLong("This is the bottom of a vast sea.");
    SetExits(([
                "north" :"/domains/town/virtual/bottom/5,1", 
                "south" : "/domains/town/virtual/bottom/5,-1", 
                "east" : "/domains/town/virtual/bottom/6,0", 
                "northeast" : "/domains/town/virtual/bottom/6,1", 
                "southeast" : "/domains/town/virtual/bottom/6,-1", 
                "west" : "/domains/town/virtual/bottom/4,0", 
                "northwest" : "/domains/town/virtual/bottom/4,1", 
                "southwest" : "/domains/town/virtual/bottom/4,-1", 
                "up" : "/domains/town/virtual/sub/5,0,10"
                ]));

    SetInventory(([
                "/domains/town/obj/stargate" : 1,
                ]));
    SetCoordinates("5,0,-11");
}

int CanReceive(object ob) {
    if(!ob) return 0;
    return room::CanReceive(ob);
}

void init(){
    ::init();
}
