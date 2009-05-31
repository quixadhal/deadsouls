#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("sharp axe");
    SetId( ({ "axe"}));
    SetAdjectives( ({ "sharp","small","hand"}));
    SetShort("a hand axe");
    SetLong("A small, sharp hand axe, suitable for close combat.");
    SetMass(150);
    SetBaseCost("silver",50);
    SetVendorType(VT_WEAPON);
    SetClass(20);
    SetDamageType(BLADE|BLUNT);
    SetWeaponType("blade");
}
