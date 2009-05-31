#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;
static void create(){
    item::create();
    SetKeyName("staff");
    SetId( ({ "staff", "stick", "pole"  }) );
    SetAdjectives( ({ "wood","wooden" }) );
    SetShort("a wooden staff");
    SetLong("This staff is old, weathered, "+
            "and lumpy. However, it feels quite sturdy "+
            "and solid.");
    SetVendorType(VT_WEAPON);
    SetDamagePoints(50);
    SetClass(100);
    SetMass(100);
    SetWeaponType("blunt");
    SetHands(2);
    SetDamageType(BLUNT);
}
