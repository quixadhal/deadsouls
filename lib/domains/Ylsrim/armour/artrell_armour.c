/*    /domains/Ylsrim/armour/artrell_armour.c
 *    from the Dead Souls V Object Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>
#include <armour_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOUR;

static void create() {
    armour::create();
    SetKeyName("leather armour");
    SetId( ({ "armour", "leather armour" }) );
    SetAdjectives( ({ "leather" }) );
    SetShort("a suit of leather armour");
    SetLong("This suit of leather armour is made specifically for the "
	    "artrell physique.");
    SetVendorType(VT_ARMOUR);
    SetMass(750);
    SetValue(150);
    SetDamagePoints(800);
    SetProtection(BLUNT, 20);
    SetProtection(BLADE, 20);
    SetProtection(KNIFE, 25);
    SetProtection(HEAT, 7);
    SetProtection(COLD, 4);
    SetProtection(SHOCK, 15);
    SetRestrictLimbs( ({ "torso", "first arm", "second arm", "third arm",
		     "fourth arm" }) );
    SetArmourType(A_ARMOUR);
}
