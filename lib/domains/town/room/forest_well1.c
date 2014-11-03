#include <medium.h>
#include <daemons.h>
#include <terrain_types.h>
#include <lib.h>
#include ROOMS_H

inherit LIB_ROOM;

int ActionFunction(){
    object *bodies = get_livings(this_object());
    if(!sizeof(bodies)) return 0;
    foreach(object body in bodies){
        int maxheal, maxstam, maxmag;
        int health, stamina, magic, poison;
        if((RACES_D->GetNonMeatRace(body->GetRace()))) continue;
        maxheal = (body->GetMaxHealthPoints()) * 0.6;
        maxstam = (body->GetMaxStaminaPoints()) * 0.6;
        maxmag = (body->GetMaxMagicPoints()) * 0.6;
        health =  (body->GetHealthPoints());
        stamina = (body->GetStaminaPoints());
        magic = (body->GetMagicPoints());

        if(health < maxheal){
            body->AddHP(random(10));
        }
        if(stamina < maxstam){
            body->AddStaminaPoints(random(10));
        }
        if(magic < maxmag){
            body->AddMagicPoints(random(10));
        }
        if(poison > 0 ){
            body->AddPoison(-(random(10)));
        }
    }
    return 1;
}

void create() {
    room::create();
    SetAmbientLight(30);
    SetDayLight(30);
    SetNightLight(30);
    SetShort("water shaft");
    SetLong("This is the vertical shaft of what seems to be a natural spring. It seems as though a bright light from below illuminates this area.");
    SetClimate("temperate");
    SetTown("wilderness");
    SetTerrainType(T_UNDERWATER);
    SetMedium(MEDIUM_WATER);
    SetItems( ([
                ({ "shaft","passage" }) : "An underwater vertical passage.",
                ]) );
    SetExits( ([
                "up" : "/domains/town/virtual/forest/-13,13",
                "down" : "/domains/town/room/forest_well2.c",
                ]) );

    SetInventory(([
                "/domains/town/obj/hspringwater" : 1,
                ]));
    set_heart_beat(5);
}
void init(){
    ::init();
}

void heart_beat(){
    ActionFunction();
}
