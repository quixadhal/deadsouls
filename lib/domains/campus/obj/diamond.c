#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("diamond");
    SetAdjectives( ({"fabled", "beautiful", "princess", "princess daphne"}) );
    SetId( ({"gem", "gemstone"}) );
    SetShort("a beautiful diamond");
    SetLong("This is the fabled Princess Daphne diamond. It is a beautiful gem.");
    SetMass(200);
    SetBaseCost("dollars", 50000);
    SetVendorType(VT_TREASURE);
    SetProperty("gem", 1);
}
void init(){
    ::init();
}
