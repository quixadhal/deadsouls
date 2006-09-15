#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("cup");
    SetId( ({"cup", "cup made of wood"}) );
    SetAdjectives( ({"simple","wooden"}) );
    SetShort("a wooden cup");
    SetLong("A simple cup made of wood, for containing fluids.");
    SetMass(5);
    SetBaseCost("silver",1);
    SetVendorType(VT_TREASURE);
    SetFlaskUses(0);
    SetStrength(5);
    SetMaxFlask(1); 
    SetMealType(MEAL_DRINK);
}
void init(){
    ::init();
}
