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
    SetLong("You are in the air above a vast sea that stretches from horizon to horizon. The stars of the night sky glitter overhead.");
    SetClimate("temperate");
    SetTown("wilderness");
    SetTerrainType(T_MIDAIR);
    SetMedium(MEDIUM_AIR);
    SetItems( ([
                ({ "sea", "ocean" }) : "A seemingly endless body of water.",
                ]) );
    SetExits( ([
                "north" : "/domains/town/virtual/sky/26,99999,1",
                "south" : "/domains/town/room/mansion_uhall1",
                "east" : "/domains/town/virtual/sky/28,99998,1",
                "down" : "/domains/town/room/mansion_ext",
                "southeast" : "/domains/town/virtual/sky/28,99997,1",
                "northeast" : "/domains/town/virtual/sky/28,99999,1",
                "west" : "/domains/town/virtual/sky/25,99998,1",
                "southwest" : "/domains/town/virtual/sky/25,99997,1",
                "northwest" : "/domains/town/virtual/sky/25,99999,1",
                "up" : "/domains/town/virtual/sky/27,99998,2",
                ]) );
}
void init(){
    ::init();
}
