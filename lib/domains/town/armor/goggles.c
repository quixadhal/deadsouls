#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("goggles");
    SetId(({"goggles"}));
    SetAdjectives(({"thick","driving","protective","eyewear"}));
    SetShort("driving goggles");
    SetLong("A set of protective goggles for wearing during "+
            "high-speed travel.");
    SetMass(5);
    SetBaseCost("silver",30);
    SetArmorType(A_VISOR);
}

void init(){
    ::init();
}

int eventReceiveDamage(mixed agent, int type, int amt, int i, mixed array l){
    if(GetWorn()){
        tell_object(environment(this_object()), "The goggles! They "+
                "do nothing!");
    }
    return ::eventReceiveDamage(agent, type, amt, i, l);          
}


