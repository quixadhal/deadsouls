#include <lib.h>
#include <vendor_types.h>
inherit "/lib/round";

void create(){
    ::create();
    SetKeyName("9mmround");
    SetId(({"round","bullet"}));
    SetAdjectives(({"9 millimeter","9mm","pistol"}));
    SetShort("a 9 millimeter pistol round");
    SetLong("This is a 9 millimeter pistol bullet. It has not been fired.");
    SetMillimeter(9);
    SetFirearmType("auto");
    SetAmmoType("acp");
    SetVendorType(VT_TREASURE);
}
