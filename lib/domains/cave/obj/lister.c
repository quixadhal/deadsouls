#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("lister bag");
    SetId( ({"lister","bag","tripod"}) );
    SetAdjectives( ({"large","canvas","wood","suspended"}) );
    SetShort("a lister bag");
    SetLong("A large canvas bag suspended on a tripod containing "+
            "drinking water.");
    SetMass(2000);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetFlaskContents("water");
    SetFlaskUses(1000);
    SetStrength(5);
    SetMaxFlask(1000); 
    SetMealType(MEAL_DRINK);
    SetTapped(1);
    SetPreventGet("This structure is not movable.");
}

void init(){
    ::init();
}

mixed CanDrink(object who, string what){
    return "This is a tapped "+GetKeyName()+". You fill something "+
        "with it, you don't drink directly from it, you barbarian.";
}
