#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOR;

static void create() {
    armor::create();
    SetKeyName("steel shield");
    SetId( ({ "shield" }) );
    SetAdjectives( ({ "medium", "steel","sturdy" }) );
    SetShort("a steel medium shield");
    SetLong("A medium shield made of steel. It is of sturdy construction.");
    SetDamagePoints(75);
    SetVendorType(VT_ARMOR);
    SetMass(100);
    SetDollarCost(450);
    SetArmorType(A_SHIELD);
    SetProtection(BLUNT, 10);
    SetProtection(BLADE, 10);
    SetProtection(KNIFE, 10);
}

