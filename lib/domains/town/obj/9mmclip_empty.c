#include <lib.h>
#include <vendor_types.h>
inherit "/lib/clip";
void create(){
    ::create();
    SetKeyName("9mmclip");
    SetId(({"clip","magazine"}));
    SetAdjectives(({"9mm","ammunition","ammo","9 millimeter","pistol"}));
    SetShort("a 9 millimeter pistol ammunition clip");
    SetLong("This is a slender, spring-loaded metal container designed "+
            "to feed a 9 millimeter pistol with bullets.");
    SetMillimeter(9);
    SetMaxAmmo(15);
    SetMass(5);
    SetBaseCost("silver", 15);
    SetAmmoType("acp");
    SetVendorType(VT_TREASURE);
}

void init(){
    ::init();
}
