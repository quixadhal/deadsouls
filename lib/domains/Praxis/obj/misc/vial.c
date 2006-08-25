#include <lib.h>
#include <vendor_types.h>
inherit LIB_MEAL_POISON;
inherit LIB_MEAL;

void create() {
    meal::create();
    poison::create();
    SetKeyName("vial");
    SetId( ({ "vial", "vial of poison", "poison" }) );
    SetShort( "Vial of poison");
    SetLong( "A glass vial with a skull and crossbones on it.");
    SetPoisonStrength(8);
    SetVendorType(VT_HERB);
    SetValue( 150);
    SetMass( 70);

}
