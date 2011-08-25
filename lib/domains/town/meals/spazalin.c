#include <lib.h>
#include ROOMS_H
#include <meal_types.h>
#include <vendor_types.h>

inherit LIB_POTION;

static void create() {
    ::create();
    SetKeyName("spazalin");
    SetId(({"pill","stimulant"}));
    SetShort("a spazalin pill");
    SetAdjectives(({"spazalin","small","red"}));
    SetLong("A small red stimulant.");
    SetMass(1);
    SetStrength(1);
    SetBaseCost("gold",3);
    SetStrength(1);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You swallow a pill.",
            "$N swallows a pill.");
    SetDuration(60);
    SetStats( ([ "speed" : 1 ]) );
    SetPoints( ([ "caffeine" : 40 ]) );
    SetVendorType(VT_MAGIC);
}

void init(){
    ::init();
}
