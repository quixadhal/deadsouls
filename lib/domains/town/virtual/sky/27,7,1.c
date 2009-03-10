#include <medium.h>
#include <terrain_types.h>
#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

void create() {
    room::create();
    SetAmbientLight(30);
    SetDayLight(30);
    SetNightLight(30);
    SetShort("The sky");
    SetLong("You are in the air above a fortress gate.");
    SetClimate("temperate");
    SetTown("wilderness");
    SetTerrainType(T_MIDAIR);
    SetMedium(MEDIUM_AIR);
    SetItems( ([
                ({ "sea", "ocean" }) : "A seemingly endless body of water.",
                ]) );
    SetExits( ([
                "north" : "/domains/town/virtual/sky/27,8,1",
                "south" : "/domains/town/virtual/sky/27,6,1",
                "east" : "/domains/town/virtual/sky/28,7,1",
                "southeast" : "/domains/town/virtual/sky/28,6,1",
                "northeast" : "/domains/town/virtual/sky/28,8,1",
                "west" : "/domains/town/virtual/sky/26,7,1",
                "southwest" : "/domains/town/virtual/sky/26,6,1",
                "northwest" : "/domains/town/virtual/sky/26,8,1",
                "up" : "/domains/town/virtual/sky/27,7,2",
                "down" : "/domains/fort/room/gate.c",
                ]) );
}
void init(){
    ::init();
}
