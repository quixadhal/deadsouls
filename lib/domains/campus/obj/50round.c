#include <lib.h>
#include <vendor_types.h>
inherit "/lib/round";

void create(){
    ::create();
    SetKeyName("round");
    SetId(({"round","bullet"}));
    SetAdjectives(({"50",".50","caliber","rifle"}));
    SetShort("a .50 caliber rifle bullet");
    SetLong("This mammoth bullet is nearly five inches long. Anything this bad "+
      "boy hits is in for a rough time.");
    SetCaliber(50);
    SetRifleType("bolt");
    SetPistolType("bolt");
    SetAmmoType("bolt");
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}
