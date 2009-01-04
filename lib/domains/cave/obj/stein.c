#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("grog stein");
    SetId( ({"stein", "mug"}) );
    SetAdjectives( ({"grog","fancy"}) );
    SetShort("a stein");
    SetLong("A large mug for holding lots of grog. It is quite "+
            "fancy and no doubt worth a great deal of money.");
    SetMass(30);
    SetBaseCost("silver",300);
    SetVendorType(VT_TREASURE);
    SetFlaskUses(0);
    SetStrength(5);
    SetMaxFlask(10); 
    SetMealType(MEAL_DRINK);
    SetPreventGet("Oddly, it appears extremely difficult to reach.");
}

void init(){
    ::init();
}
