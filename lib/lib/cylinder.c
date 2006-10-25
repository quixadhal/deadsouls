#include <lib.h>
#include <vendor_types.h>
inherit LIB_STORAGE;
void create(){
    ::create();
    SetKeyName("revolver cylinder");
    SetId(({"cylinder"}));
    SetShort("a revolver cylinder");
    SetLong("This is the round barrel the holds a revolver's bullets.");
    SetMass(5);
    SetValue(10);
    SetMaxCarry(10);
    SetVendorType(VT_TREASURE);
}
