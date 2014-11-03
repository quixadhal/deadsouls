#include <lib.h>
#include <vendor_types.h>
#include <meal_types.h>
inherit LIB_FLASK;

void create(){
    ::create();
    SetKeyName("bottle");
    SetId( ({"vessel","winebottle","bottle of wine","nouveau"}) );
    SetAdjectives( ({"wine","glass","nouveau"}) );
    SetShort("a bottle of wine");
    SetLong("A bottle of something or other Nouveau. The label is too stained to read.");
    SetMass(40);
    SetBaseCost("gold",2);
    SetVendorType(VT_DRINK);
    SetFlaskContents("wine");
    SetFlaskUses(10);
    SetStrength(20);
    SetMaxFlask(10); 
    SetMealType(MEAL_ALCOHOL);
    SetItems(([
                "label" : "It is attached to the bottle and is meant to be read.",
                ]));
    SetReads(([
                "label" : "It's too stained to read, all right.",
                ]));
}
void init(){
    ::init();
}
