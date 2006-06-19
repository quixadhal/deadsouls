#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("lab key");
    SetAdjectives( ({"lab", "probability lab"}) );
    SetId( ({"prob_door_key", "key", "labkey"}) );
    SetShort("lab key");
    SetLong("This is a key labeled 'probability lab'.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}
