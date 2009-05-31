#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;
static void create(){
    item::create();
    SetKeyName("golden staff");
    SetId( ({ "staff"  }) );
    SetAdjectives( ({ "golden" }) );
    SetShort("a golden staff");
    SetLong("This is a thick staff, with one end covered in "+
            "a golden metal.");
    SetVendorType(VT_WEAPON);
    SetClass(10);
    SetBaseCost("silver",800);
    SetMass(100);
    SetWeaponType("blunt");
    SetDamageType(BLUNT);
}
