#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("sharp dagger");
    SetId( ({ "dagger"}));
    SetAdjectives( ({ "sharp","fine","wicked","wicked-looking"}));
    SetShort("a wicked-looking dagger");
    SetLong("A sharp, wicked dagger.");
    SetMass(150);
    SetBaseCost("silver",50);
    SetVendorType(VT_WEAPON);
    SetClass(10);
    SetDamageType(KNIFE);
    SetWeaponType("knife");
}
void init(){
    ::init();
}
