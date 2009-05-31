#include <lib.h>
#include <vendor_types.h>
inherit "/lib/pistol";

void create(){
    pistol::create();
    SetKeyName("revolver");
    SetId(({"gun","pistol","piece"}));
    SetAdjectives(({".357","357","magnum"}));
    SetShort("a .357 magnum revolver");
    SetLong("This is a thick, heavy chunk of weaponry that fires some very "+
            "serious rounds. Bullets are loaded individually into it, and they, along with spent "+
            "shells, remain in the cylinder until unloaded.");
    SetCaliber(357);
    SetFirearmType("revolver");
    SetAmmoType("magnum");
    SetMaxAmmo(6);
    SetMass(20);
    SetDollarCost(10);
    SetVendorType(VT_WEAPON);
    SetMagnum(30);
}
void init(){
    ::init();
}
