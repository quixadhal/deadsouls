#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;
static void create(){
    item::create();
    SetKeyName("staff");
    SetId( ({ "staff", "stick", "pole"  }) );
    SetAdjectives( ({ "wood","wooden","strong","heavy","thick","sturdy","solid" }) );
    SetShort("a large wooden staff");
    SetLong("This staff is thick, strong, "+
            "and heavy. It feels like a sturdy "+
            "and solid weapon.");
    SetVendorType(VT_WEAPON);
    SetDamagePoints(50);
    SetClass(500);
    SetBaseCost("silver",25000);
    SetMass(700);
    SetWeaponType("blunt");
    SetHands(2);
    SetDamageType(BLUNT);
}
