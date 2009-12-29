#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("Fell Sword");
    SetId( ({"sword", "weapon", "fellsword"}) );
    SetAdjectives( ({"fell", "elvish", "powerful", "antique", "old"}) );
    SetShort("The Fell Sword");
    SetLong("An elvish sword of great antiquity and tremendous power.");
    SetUnique(1);
    SetMass(100);
    SetBaseCost("silver", 5000);
    SetVendorType(VT_WEAPON);
    SetClass(40);
    SetDamageType(BLADE);
    SetWeaponType("blade");
}
void init(){
    ::init();
}
