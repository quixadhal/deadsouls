#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("sharp sword");
    SetId( ({ "sword", "short sword", "shortsword"}));
    SetAdjectives( ({ "short","sharp","fine"}));
    SetShort("a sharp sword");
    SetLong("A fine, sharp, short sword.");
    SetMass(300);
    SetBaseCost("silver",50);
    SetVendorType(VT_WEAPON);
    SetClass(150);
    SetDamageType(BLADE);
    SetWeaponType("blade");
}
