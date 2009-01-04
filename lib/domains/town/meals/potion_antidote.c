#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>

inherit LIB_POTION;

static void create() {
    potion::create();
    SetKeyName("antidote");
    SetId( ({ "vial","bottle", "antidote","philtrum","antidote to poison","vial of antidote" }) );
    SetAdjectives( "antidote" );
    SetShort("a vial of antidote");
    SetLong("A vial containing a clear blue liquid that counteracts poison.");
    SetMass(20);
    SetMealType(MEAL_DRINK);
    SetStrength(5);
    SetMealMessages("You drink from the vial.",
            "$N drinks from a vial.");
    SetBaseCost("silver",200);
    SetPoints( ([ "poison" : -10 ]) );
    SetEmptyName("vial");
    SetEmptyShort("an empty vial");
    SetEmptyLong("This empty vial once contained some liquid.");
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}
