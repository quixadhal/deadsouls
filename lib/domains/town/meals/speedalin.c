#include <lib.h>
#include ROOMS_H
#include <meal_types.h>
#include <vendor_types.h>

inherit LIB_POTION;

static void create() {
    ::create();
    SetKeyName("speedalin");
    SetId(({"pill","stimulant"}));
    SetShort("a speedalin pill");
    SetAdjectives(({"speedalin","small","white"}));
    SetLong("A small white stimulant.");
    SetMass(1);
    SetStrength(1);
    SetBaseCost("gold",2);
    SetStrength(1);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You swallow a pill.",
            "$N swallows a pill.");
    SetDuration(20);
    SetStats( ([ "speed" : 1 ]) );
    SetPoints( ([ "caffeine" : 30 ]) );
    SetVendorType(VT_MAGIC);
}

void init(){
    ::init();
}
