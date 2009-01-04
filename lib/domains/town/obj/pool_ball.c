#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("billiards ball");
    SetId( ({"ball"}) );
    SetAdjectives( ({"pool","billiards","billiard","heavy"}) );
    SetShort("a billiards ball");
    SetLong("This is a heavy ball, slighly smaller than palm-sized, "
            "used in the game of billiards.");
    SetMass(30);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
}

void init(){
    ::init();
}
