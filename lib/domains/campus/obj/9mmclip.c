#include <lib.h>
#include <vendor_types.h>
inherit LIB_CLIP;

void create(){
    clip::create();
    SetKeyName("9mm clip");
    SetId(({"clip","magazine"}));
    SetAdjectives(({"9mm","ammunition","ammo","9 millimeter","pistol"}));
    SetShort("a 9 millimeter pistol ammunition clip");
    SetLong("This is a slender, spring-loaded metal container designed "+
            "to feed a 9 millimeter pistol with bullets.");
    SetMillimeter(9);
    SetMaxAmmo(15);
    SetAmmoType("acp");
    SetVendorType(VT_TREASURE);
    SetInventory(([
                "/domains/campus/obj/9mmround" : 15
                ]));
}

void init(){
    clip::init();
}
