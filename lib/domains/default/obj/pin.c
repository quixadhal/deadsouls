#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("pull-pin");
    SetId(({"pin","ring"}));
    SetAdjectives(({"pull","grenade","fuse"}));
    SetShort("a grenade pin");
    SetLong("This is the pull pin from a hand grenade fuse.");
    SetMass(1);
    SetDollarCost(0);
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}
