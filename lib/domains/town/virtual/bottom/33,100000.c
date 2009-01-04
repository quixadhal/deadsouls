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
                "north" :"/domains/town/virtual/bottom/33,1", 
                "south" : "/domains/town/virtual/bottom/33,99999", 
                "east" : "/domains/town/virtual/bottom/34,100000", 
                "northeast" : "/domains/town/virtual/bottom/34,100000", 
                "southeast" : "/domains/town/virtual/bottom/34,100000", 
                "west" : "/domains/town/virtual/bottom/32,100000", 
                "northwest" : "/domains/town/virtual/bottom/100000,100000", 
                "southwest" : "/domains/town/virtual/bottom/100000,100000", 
                "up" : "/domains/town/virtual/sub/33,100000,10"
                ]));

    SetInventory(([
                "/domains/town/obj/stargate" : 1,
                ]));

}
int CanReceive(object ob) {
    if(!ob) return 0;
    return room::CanReceive(ob);
}

void init(){
    ::init();
}
