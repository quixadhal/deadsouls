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
    SetLong("You are in the air above Fort Road.");
    SetClimate("temperate");
    SetTown("wilderness");
    SetTerrainType(T_MIDAIR);
    SetMedium(MEDIUM_AIR);
    SetItems( ([
                ({ "sea", "ocean" }) : "A seemingly endless body of water.",
                ]) );
    SetExits( ([
                "north" : "/domains/town/virtual/sky/28,7,1",
                "south" : "/domains/town/virtual/sky/28,5,1",
                "east" : "/domains/town/virtual/sky/29,6,1",
                "southeast" : "/domains/town/virtual/sky/29,5,1",
                "northeast" : "/domains/town/virtual/sky/29,7,1",
                "west" : "/domains/town/virtual/sky/27,6,1",
                "southwest" : "/domains/town/virtual/sky/27,5,1",
                "northwest" : "/domains/town/virtual/sky/27,7,1",
                "up" : "/domains/town/virtual/sky/28,6,2",
                "down" : "/domains/fort/room/f_road1.c",
                ]) );
}
void init(){
    ::init();
}
