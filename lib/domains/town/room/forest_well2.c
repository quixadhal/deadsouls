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
        maxheal = (body->GetMaxHealthPoints()) * 0.85;
        maxstam = (body->GetMaxStaminaPoints()) * 0.85;
        maxmag = (body->GetMaxMagicPoints()) * 0.85;
        health =  (body->GetHealthPoints());
        stamina = (body->GetStaminaPoints());
        magic = (body->GetMagicPoints());

        if(health < maxheal){
            body->AddHP(random(10));
        }
        if(stamina < maxstam){
            body->AddStaminaPoints(random(15));
        }
        if(magic < maxmag){
            body->AddMagicPoints(random(15));
        }
        if(poison > 0 ){
            body->AddPoison(-(random(15)));
        }
    }
    return 1;
}

void create() {
    room::create();
    SetAmbientLight(30);
    SetDayLight(30);
    SetNightLight(30);
    SetShort("Spring bottom");
    SetLong("This is the bedrock floor of a natural spring. A passage leads up.\n%^BLUE%^There is a large, glowing egg here.%^RESET%^");
    SetClimate("temperate");
    SetTown("wilderness");
    SetTerrainType(T_SEAFLOOR);
    SetMedium(MEDIUM_WATER);
    SetItems( ([
                ({ "shaft","passage" }) : "An underwater vertical passage.",
                ({ "egg","large egg","large glowing egg","glowing egg" }) : "About a meter in diameter, this large egg seems to glow and pulsate with energy.",
                ]) );
    SetExits(([
                "up" : "/domains/town/room/forest_well1",
                ]));

    SetInventory(([
                "/domains/town/obj/hspringwater" : 1,
                "/domains/town/npc/giant_isopod" : 1,
                ]));
    set_heart_beat(6);
}
void init(){
    ::init();
}

void heart_beat(){
    ActionFunction();
}
