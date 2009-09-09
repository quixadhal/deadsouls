#include <lib.h>
#include <vendor_types.h>
inherit LIB_ROUND;

void create(){
    round::create();
    SetKeyName(".50 round");
    SetId(({"round","bullet"}));
    SetAdjectives(({".50","caliber","rifle"}));
    SetShort("a .50 caliber rifle bullet");
    SetLong("This mammoth bullet is nearly five inches long. Anything this bad "+
            "boy hits is in for a rough time.");
    SetCaliber(50);
    SetFirearmType("bolt");
    SetFirearmType("bolt");
    SetAmmoType("bolt");
    SetVendorType(VT_TREASURE);
    SetMass(1);
}
