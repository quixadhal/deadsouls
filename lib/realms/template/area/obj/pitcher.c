#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("pitcher");
    SetId( ({"pitcher", "vessel", "pitcher made of clay"}) );
    SetAdjectives( ({"small","clay","vessel"}) );
    SetShort("a small clay pitcher");
    SetLong("A small, clay vessel for containing fluids. You may "+
            "want to \"pour from pitcher into cup\".");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetFlaskContents("water");
    SetFlaskUses(2);
    SetStrength(5);
    SetMaxFlask(3); 
    SetMealType(MEAL_DRINK);
}
void init(){
    ::init();
}
