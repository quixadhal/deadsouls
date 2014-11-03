#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("bottle");
    SetId( ({"whiskey","vessel","bottle of whiskey","rebel yell"}) );
    SetAdjectives( ({"whiskey","glass","rebel yell"}) );
    SetShort("a bottle of whiskey");
    SetLong("A bottle of whiskey.");
    SetMass(40);
    SetBaseCost("gold",3);
    SetVendorType(VT_DRINK);
    SetFlaskContents("whiskey");
    SetFlaskUses(8);
    SetStrength(30);
    SetMaxFlask(10); 
    SetMealType(MEAL_ALCOHOL);
    SetItems(([
                "label" : "It is attached to the bottle and has a picture of "+
                "a mounted soldier in a gray uniform, on a rearing horse, "+
                "sword raised.",
                ]));
    SetReads(([
                "label" : "REBEL YELL",
                ]));
}
void init(){
    ::init();
}
