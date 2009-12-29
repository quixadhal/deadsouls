#include <lib.h>
#include <vendor_types.h>
inherit LIB_ROUND;

void create(){
    round::create();
    SetKeyName(".357 round");
    SetId(({"round","bullet"}));
    SetAdjectives(({".357","caliber","revolver","pistol"}));
    SetShort("a .357 pistol round");
    SetLong("This is a .357 caliber revolver bullet. It has not been fired.");
    SetCaliber(357);
    SetFirearmType("revolver");
    SetAmmoType("magnum");
    SetVendorType(VT_TREASURE);
    SetMass(1);
}
