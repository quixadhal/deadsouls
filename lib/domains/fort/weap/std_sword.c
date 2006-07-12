#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("sword");
    SetId( ({ "sword", "standard sword" }) );
    SetAdjectives( ({ "standard-issue" }) );
    SetShort("a sword");
    SetLong("A standard-issue sword.");
    SetMass(150);
    SetDollarCost(50);
    SetVendorType(VT_WEAPON);
    SetClass(30);
    SetDamageType(BLADE);
    SetWeaponType("blade");
}
void init(){
    ::init();
}
