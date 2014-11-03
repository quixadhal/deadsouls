#include <medium.h>
#include <daemons.h>
#include <terrain_types.h>
#include <lib.h>
#include <climb.h>
#include ROOMS_H

inherit LIB_ROOM;
inherit LIB_CLIMB;

int OutClimb(){
    eventClimb(this_player(),CLIMB_OUT,"/domains/town/virtual/forest/-13,12");
}

int ActionFunction(){
    object *bodies = get_livings(this_object());
    foreach(object body in bodies){
        int maxheal, maxstam, maxmag;
        int health, stamina, magic, poison;
        if((RACES_D->GetNonMeatRace(body->GetRace()))) continue;
        maxheal = (body->GetMaxHealthPoints()) / 2;
        maxstam = (body->GetMaxStaminaPoints()) / 2;
        maxmag = (body->GetMaxMagicPoints()) / 2;
        health =  (body->GetHealthPoints());
        stamina = (body->GetStaminaPoints());
        magic = (body->GetMagicPoints());

        if(health < maxheal){
            body->AddHP(random(5));
        }
        if(stamina < maxstam){
            body->AddStaminaPoints(random(5));
        }
        if(magic < maxmag){
            body->AddMagicPoints(random(5));
        }
        if(poison > 0 ){
            body->AddPoison(-(random(5)));
        }
    }
    return 1;
}

void create() {
    room::create();
    SetAmbientLight(30);
    SetDayLight(30);
    SetNightLight(30);
    SetShort("a glowing pool");
    SetLong("This is a strange, glowing pool in the middle of the forest.");
    SetClimate("temperate");
    SetTown("wilderness");
    SetTerrainType(T_SURFACE);
    SetMedium(MEDIUM_SURFACE);
    SetNoSink(1);
    SetItems( ([
                ({ "forest" }) : "It surrounds the pool.",
                ]) );
    SetExits( ([
                "south" : "/domains/town/virtual/forest/-13,12",
                "north" : "/domains/town/virtual/forest/-13,14",
                "east" : "/domains/town/virtual/forest/-12,13",
                "southeast" : "/domains/town/virtual/forest/-12,12",
                "northeast" : "/domains/town/virtual/forest/-12,14",
                "out" : "/domains/town/virtual/forest/-14,12",
                "west" : "/domains/town/virtual/forest/-14,13",
                "southwest" : "/domains/town/virtual/forest/-14,12",
                "northwest" : "/domains/town/virtual/forest/-14,14",
                "down" : "/domains/town/room/forest_well1.c",
                ]) );
    SetInventory(([
                "/domains/town/obj/hpoolwater" : 1,
                "/domains/town/npc/nymph" : 1,
                ]));
    SetFlyRoom("/domains/town/virtual/sky/-13,13,1");
    SetClimb( (: OutClimb :) , CLIMB_OUT);
    set_heart_beat(5);
    SetObviousExits("climb out");
}
void init(){
    ::init();
}

void heart_beat(){
    ActionFunction();
}
