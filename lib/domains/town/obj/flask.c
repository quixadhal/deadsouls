#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("flask");
    SetId( ({"flask"}) );
    SetAdjectives( ({"small","metal","vessel"}) );
    SetShort("a small flask");
    SetLong("A small, metal vessel for containing fluids.");
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
