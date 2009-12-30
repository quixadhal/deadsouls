#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("sharp sword");
    SetId( ({"sword"}) );
    SetAdjectives( ({ "very", "short","sharp","fine"}));
    SetShort("a very sharp sword");
    SetLong("A very fine, sharp sword. It is solidly crafted "+
            "and well balanced.");
    SetMass(400);
    SetBaseCost("silver", 2500);
    SetVendorType(VT_WEAPON);
    SetClass(35);
    SetDamageType(BLADE);
    SetWeaponType("blade");
}

void init(){
    ::init();
}
