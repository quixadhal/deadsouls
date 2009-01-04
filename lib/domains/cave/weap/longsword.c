#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("longsword");
    SetId( ({ "sword","weapon" }) );
    SetAdjectives( ({ "long","well crafted","bladed","two handed" }) );
    SetShort("a longsword");
    SetLong("A well crafted bladed weapon of exceptional lethality. It "+
            "is designed to be wielded with two hands.");
    SetMass(500);
    SetHands(2);
    SetBaseCost("silver",2800);
    SetVendorType(VT_WEAPON);
    SetClass(35);
    SetDamageType(BLADE);
    SetWeaponType("blade");
}

void init(){
    ::init();
}
