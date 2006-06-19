#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("rotten apple");
    SetId( ({"apple","fruit"}) );
    SetAdjectives( ({"rotten","foul-smelling","dried-up","browned","brown"}) );
    SetShort("a rotten apple");
    SetLong("This is browned, foul-smelling, dried-up apple.");
    SetMass(5);
    SetDollarCost(0);
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}
