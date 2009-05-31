#include <lib.h>
#include <vendor_types.h>
inherit "/lib/battery";
void create(){
    ::create();
    SetKeyName("d-cell");
    SetId( ({"cell","battery"}) );
    SetAdjectives( ({"d","generic","D","D-cell"}) );
    SetShort("a D cell battery");
    SetLong("This is a typical D-cell battery, of a generic brand.");
    SetMass(2);
    SetBaseCost("silver",2);
    SetVendorType(VT_TREASURE);
    SetPowerType("DC");
    SetCellType("D");
    SetCharge(10000);
    SetRechargeable(0);
    SetDrainable(1);
    SetDrainRate(1);
}

void init(){
    ::init();
}
