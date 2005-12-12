#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("butcher knife");
    SetId( ({ "knife"}));
    SetAdjectives( ({ "sharp","razor sharp","steel","hefty","stainless","butcher"}));
    SetShort("an 8-inch butcher knife");
    SetLong("This is an 8-inch butcher knife. This razor sharp knife has been forged from molded and hammered high-carbon stainless steel. It is solid, hefty, and well-balanced.");
    SetMass(50);
    SetBaseCost("silver",95);
    SetVendorType(VT_WEAPON);
    SetClass(20);
    SetDamageType(BLADE);
    SetWeaponType("blade");
}

int GetFancy(){
    return 1;
}
