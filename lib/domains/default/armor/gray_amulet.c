// The carrier of this amulet will not be forced to quit
// if they hit their max idle time. Also speeds magic point refresh.

#include <lib.h>
#include ROOMS_H
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("gray amulet");
    SetId(({"amulet", "gray", "idler_amulet" }));
    SetShort("a gray amulet");
    SetLong("This dull, heavy amulet seems rather worthless, though "
            "something about it seems to suggest hidden purpose.");
    SetProperties(([
                "no steal" : 1,
                ]));
    SetMass(50);
    SetBaseCost(100000);
    SetArmorType(A_AMULET);
    SetRetainOnDeath(1);
    SetRestrictLimbs( ({ "torso" }) );
    set_heart_beat(7);
}

void init(){
    ::init();
}

void heart_beat(){
    object env = environment();
    if(env && living(env) && GetWorn()){
        env->AddMagicPoints(1);
    }
}

