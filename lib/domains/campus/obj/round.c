#include <lib.h>
#include <vendor_types.h>
inherit "/lib/round";

void create(){
    ::create();
    SetKeyName("357round");
    SetId(({"round","bullet"}));
    SetAdjectives(({"357",".357","pistol"}));
    SetShort("a .357 pistol round");
    SetLong("This is a .357 caliber revolver bullet. It has not been fired.");
    SetCaliber(357);
    SetPistolType("revolver");
    SetAmmoType("magnum");
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}
