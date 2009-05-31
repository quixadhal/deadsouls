#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("weight");
    SetAdjectives( ({"iron", "heavy"}) );
    SetId( ({"iron", "item", "thing"}) );
    SetShort("an iron weight");
    SetLong("This is an item made of iron whose only purpose is being heavy.");
    SetMass(1800);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
}

void init(){
    ::init();
}
