#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("stein");
    SetId( ({"mug","vessel","guinness"}) );
    SetAdjectives( ({"large","beer","guinness"}) );
    SetShort("a large beer stein");
    SetLong("A large vessel for drinking fluids.");
    SetMass(40);
    SetBaseCost("gold",1);
    SetVendorType(VT_DRINK);
    SetFlaskContents("Guinness");
    SetFlaskUses(10);
    SetStrength(10);
    SetMaxFlask(10); 
    SetMealType(MEAL_ALCOHOL|MEAL_DRINK|MEAL_FOOD);
}
void init(){
    ::init();
}
