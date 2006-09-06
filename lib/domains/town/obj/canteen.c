#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("canteen");
    SetId( ({"canteen","vessel"}) );
    SetAdjectives( ({"plastic","green"}) );
    SetShort("a green plastic canteen");
    SetLong("A plastic vessel for containing fluids.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetFlaskContents("water");
    SetFlaskUses(10);
    SetStrength(5);
    SetMaxFlask(10); 
    SetMealType(MEAL_DRINK);
}
void init(){
    ::init();
}
