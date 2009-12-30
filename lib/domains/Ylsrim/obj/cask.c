#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("cask");
    SetId( ({"cask","keg"}) );
    SetAdjectives( ({"large","wooden","wood"}) );
    SetShort("a wooden cask");
    SetLong("A large wooden cask for containing fluids.");
    SetMass(2000);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetFlaskContents("wine");
    SetFlaskUses(100);
    SetStrength(10);
    SetMaxFlask(100); 
    SetMealType(MEAL_ALCOHOL);
    SetUnique(1);
    SetTapped(1);
}

void init(){
    ::init();
}
