#include <lib.h>
#include <daemons.h>
#include <medium.h>
#include <terrain_types.h>

inherit LIB_ROOM;

mixed CanGo(object who, string dir) {
    return CanFly(who, dir);
}

static void create() {
    object ob;
    room::create();
    SetAmbientLight(30);
    SetNoClean(1);
    SetTerrainType(T_SPACE);
    SetMedium(MEDIUM_SPACE);

    SetShort("Outer Space");
    SetLong("Outer space. A big, place. There is a medium sized planet below.");
    SetExits(([
                "+x" : "/domains/town/virtual/space/1,2,1", 
                "-x-y-z" : "/domains/town/virtual/space/2100000000,2100000000,0", 
                "-z" : "/domains/town/virtual/space/1,1,0", 
                "-x-y" : "/domains/town/virtual/space/2100000000,2100000000,1", 
                "+x+y-z" : "/domains/town/virtual/space/2,2,0", 
                "-y" : "/domains/town/virtual/space/2100000000,1,1", 
                "+x-y+z" : "/domains/town/virtual/space/2100000000,2,2", 
                "-x+y+z" : "/domains/town/virtual/space/2,2100000000,2", 
                "-x" : "/domains/town/virtual/space/1,2100000000,1", 
                "+x+y" : "/domains/town/virtual/space/2,2,1", 
                "-x-y+z" : "/domains/town/virtual/space/2100000000,2100000000,2", 
                "+x-y-z" : "/domains/town/virtual/space/2100000000,2,0", 
                "-x+y-z" : "/domains/town/virtual/space/2,2100000000,0", 
                "+z" : "/domains/town/virtual/space/1,1,2", 
                "+x-y" : "/domains/town/virtual/space/2100000000,2,1", 
                "-x+y" : "/domains/town/virtual/space/2,2100000000,1", 
                "+y" : "/domains/town/virtual/space/2,1,1", 
                "+x+y+z" : "/domains/town/virtual/space/2,2,2",
                "down": "/domains/town/virtual/sky/1,1,1000",
                "-y+z" : "/domains/town/virtual/space/2100000000,1,2", 
                "+x-z" : "/domains/town/virtual/space/1,2,0", 
                "-x-z" : "/domains/town/virtual/space/1,2100000000,0", 
                "+y-z" : "/domains/town/virtual/space/2,1,0", 
                "+x+z" : "/domains/town/virtual/space/1,2,2", 
                "-y-z" : "/domains/town/virtual/space/2100000000,1,0", 
                "-x+z" : "/domains/town/virtual/space/1,2100000000,2", 
                "+y+z" : "/domains/town/virtual/space/2,1,2", 
                ]) );

    SetInventory(([
                "/domains/town/obj/stargate2" : 1,
                ]));
}
int CanReceive(object ob) {
    if(!ob) return 0;
    return room::CanReceive(ob);
}

void init(){
    ::init();
}
