#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("sharp sword");
    SetId( ({"sword"}) );
    SetAdjectives( ({ "short","sharp","fine"}));
    SetShort("a sharp sword");
    SetLong("A fine, sharp sword. It is solidly crafted and well balanced.");
    SetMass(300);
    SetBaseCost("silver", 1100);
    SetVendorType(VT_WEAPON);
    SetClass(35);
    SetDamageType(BLADE);
    SetWeaponType("blade");
}
void init(){
    ::init();
}
