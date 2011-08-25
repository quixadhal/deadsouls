#include <lib.h>
#include ROOMS_H
#include <meal_types.h>
#include <vendor_types.h>

inherit LIB_POTION;

static void create() {
    ::create();
    SetKeyName("red spice");
    SetId(({"spice","stick","stick of red spice","slender"}));
    SetShort("a stick of red spice");
    SetAdjectives(({"red","spice"}));
    SetLong("A slender stick of red spice.");
    SetMass(1);
    SetStrength(1);
    SetBaseCost("gold",8);
    SetStrength(1);
    SetMealType(MEAL_FOOD);
    SetMealMessages("You eat a stick of red spice.",
            "$N eats a stick of red spice.");
    SetDuration(1);
    SetPoints( ([ "HP" : 300 ]) );
    SetVendorType(VT_MAGIC);
}

void init(){
    ::init();
}
