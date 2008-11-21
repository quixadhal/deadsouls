#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("mug");
    SetId( ({"mug", "mug made of clay"}) );
    SetAdjectives( ({"simple","clay"}) );
    SetShort("a clay mug");
    SetLong("A simple mug made of clay, for containing fluids.");
    SetMass(5);
    SetBaseCost("silver",1);
    SetVendorType(VT_TREASURE);
    SetFlaskUses(0);
    SetStrength(5);
    SetMaxFlask(2); 
    SetMealType(MEAL_DRINK);
}
void init(){
    ::init();
}
