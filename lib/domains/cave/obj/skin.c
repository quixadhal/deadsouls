#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("skin");
    SetId( ({"skin","vessel","bladder","pouch"}) );
    SetAdjectives( ({"skin","bladder","water","animal"}) );
    SetShort("a water skin");
    SetLong("A pouch made of animal bladder for containing fluids.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetFlaskContents("water");
    SetFlaskUses(20);
    SetStrength(5);
    SetMaxFlask(20); 
    SetMealType(MEAL_DRINK);
}

void init(){
    ::init();
}
