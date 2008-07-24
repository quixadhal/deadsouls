#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("small knife");
    SetId( ({ "knife"}));
    SetAdjectives( ({ "small","fine"}));
    SetShort("a small knife");
    SetLong("A knife.");
    SetMass(50);
    SetBaseCost("silver",5);
    SetVendorType(VT_WEAPON);
    SetClass(5);
    SetDamageType(KNIFE);
    SetWeaponType("knife");
}

void init(){
    ::init();
}
