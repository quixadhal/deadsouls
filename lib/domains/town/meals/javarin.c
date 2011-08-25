#include <lib.h>
#include ROOMS_H
#include <meal_types.h>
#include <vendor_types.h>

inherit LIB_POTION;

static void create() {
    ::create();
    SetKeyName("javarin");
    SetId(({"pill","stimulant"}));
    SetShort("a javarin pill");
    SetAdjectives(({"javarin","small","yellow"}));
    SetLong("A small yellow stimulant.");
    SetMass(1);
    SetStrength(1);
    SetBaseCost("gold",1);
    SetStrength(1);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You swallow a pill.",
            "$N swallows a pill.");
    SetDuration(10);
    SetStats( ([ "speed" : 1 ]) );
    SetPoints( ([ "caffeine" : 20 ]) );
    SetVendorType(VT_MAGIC);
}

void init(){
    ::init();
}
