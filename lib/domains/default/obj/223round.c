#include <lib.h>
#include <vendor_types.h>
inherit LIB_ROUND;


void create(){
    round::create();
    SetKeyName("223round");
    SetId(({"round","bullet"}));
    SetAdjectives(({".223","caliber","rifle","m16","M16","m-16","M-16"}));
    SetShort("a .223 caliber rifle round");
    SetLong("This is a .223 caliber rifle round, probably for an M-16 assault rifle. "+
            "It is not very wide, but contains a large powder charge. It is no doubt a powerful "+
            "piece of ammunition. It has not been fired.");
    SetCaliber(223);
    SetFirearmType("auto");
    SetAmmoType("nato");
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}
