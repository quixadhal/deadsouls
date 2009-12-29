#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("cup");
    SetId( ({"cup", "trophy"}) );
    SetAdjectives( ({"trophy", "brass", "small"}) );
    SetShort("a small trophy cup");
    SetLong("About the size of a teacup, this brass cup appears to be some sort of trophy from a competition. There are words engraved on it.");
    SetItems(([
                ({ "word", "words", "engraving", "inscription" }) : "Words engraved on the cup.",
                ]));
    SetRead(([
                "default" : "Try: read inscription on cup",
                ({ "word", "words", "inscription", "engraving" }) : "3rd Runner Up, Junior Miss Cocoa Butter Pageant, 1963",
                ]));
    SetMass(10);
    SetBaseCost("silver",100);
    SetVendorType(VT_TREASURE);
    SetFlaskUses(0);
    SetStrength(5);
    SetMaxFlask(1); 
    SetMealType(MEAL_DRINK);
}

void init(){
    ::init();
}
