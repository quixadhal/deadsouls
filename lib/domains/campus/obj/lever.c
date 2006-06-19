#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("hand grenade spoon");
    SetId(({"lever","spoon"}));
    SetAdjectives(({"grenade","hand grenade","metal","small"}));
    SetShort("a small metal lever");
    SetLong("This is the fuse lever, or spoon, for a hand grenade.");
    SetMass(1);
    SetDollarCost(0);
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}
