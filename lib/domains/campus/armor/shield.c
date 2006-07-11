/*    /domains/Examples/armor/shield.c
 *    from the Dead Souls LPC Library
 *    an example shield
 *    created by Lassondra@Dead Souls
 */

#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOR;

static void create() {
    armor::create();
    SetKeyName("large shield");
    SetId( ({ "shield" }) );
    SetAdjectives( ({ "wood", "wooden","large" }) );
    SetShort("a large wooden shield");
    SetLong("A large, heavy shield made of strong wood banded with iron.");
    SetDamagePoints(75);
    SetVendorType(VT_ARMOR);
    SetMass(100);
    SetDollarCost(150);
    SetArmorType(A_SHIELD);
    SetProtection(BLUNT, 10);
    SetProtection(BLADE, 10);
    SetProtection(KNIFE, 10);
    //SetRestrictLimbs( ({"left hand"}) );
}

