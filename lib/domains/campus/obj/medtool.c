#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("medtool");
    SetId( ({"tool"}) );
    SetAdjectives( ({"yautja","med","medical"}) );
    SetShort("a yautja medtool");
    SetLong("This is pistol-shaped metallic device used in cases of "+
            "medical emergency. It appears you can try to apply it to yourself.");
    SetMass(100);
    SetDollarCost(1000);
    SetVendorType(VT_TREASURE);
}

void init(){
    ::init();
}
