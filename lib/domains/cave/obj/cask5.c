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
    SetFlaskContents("grog");
    SetFlaskUses(1000);
    SetStrength(5);
    SetMaxFlask(1000); 
    SetMealType(MEAL_ALCOHOL);
    SetTapped(1);
    SetPreventGet("The cask is firmly attached to the floor.");
}

void init(){
    ::init();
}

mixed CanDrink(object who, string what){
    return "This is a tapped "+GetKeyName()+". You fill something "+
        "with it, you don't drink directly from it, you barbarian.";
}

