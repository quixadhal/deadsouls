#include <lib.h>
#include <vendor_types.h>
inherit LIB_ROUND;

void create(){
    round::create();
    SetKeyName(".45 round");
    SetId(({"round","bullet"}));
    SetAdjectives(({".45","caliber","acp","pistol","semiautomatic"}));
    SetShort("a .45 caliber pistol round");
    SetLong("This is a .45 caliber bullet for a semiautomatic pistol. It has not been fired.");
    SetCaliber(45);
    SetFirearmType("auto");
    SetAmmoType("acp");
    SetVendorType(VT_TREASURE);
    SetMass(1);
}
