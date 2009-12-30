#include <lib.h>
#include <vendor_types.h>
inherit LIB_CLIP;

void create(){
    clip::create();
    SetKeyName(".223 clip");
    SetId(({"clip","magazine"}));
    SetAdjectives(({".223","rifle","caliber","ammo","ammunition"}));
    SetShort("a .223 caliber rifle ammunition clip");
    SetLong("This is a spring-loaded ammunition clip for a .223 caliber "+
            "rifle. It will contain a maximum of thirty rounds.");
    SetCaliber(223);
    SetMaxAmmo(30);
    SetMass(5);
    SetAmmoType("nato");
    SetVendorType(VT_TREASURE);
    SetInventory(([
                "/domains/town/obj/223round" : 30,
                ]));
    SetBaseCost("silver", 75);
}
void init(){
    ::init();
}
