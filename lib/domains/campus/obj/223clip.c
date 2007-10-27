#include <lib.h>
#include <vendor_types.h>
inherit "/lib/clip";

void create(){
    ::create();
    SetKeyName("223clip");
    SetId(({"clip","magazine"}));
    SetAdjectives(({"223",".223","ammunition","rifle","caliber","ammo","ammunition"}));
    SetShort("a .223 caliber rifle ammunition clip");
    SetLong("This is a spring-loaded ammunition clip for a .223 caliber "+
      "rifle. It will contain a maximum of thirty rounds.");
    SetCaliber(223);
    SetMaxAmmo(30);
    SetAmmoType("nato");
    SetVendorType(VT_TREASURE);
    SetInventory(([
        "/domains/campus/obj/223round" : 30,
      ]));
}
void init(){
    ::init();
}
