/*    /domains/Ylsrim/armour/helm.c
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
    SetKeyName("wooden helm");
    SetId("helm");
    SetAdjectives("wooden");
    SetShort("a wooden helm");
    SetLong("It is a lame wooden helm.");
    SetDamagePoints(3000);
    SetVendorType(VT_ARMOUR);
    SetMass(200);
    SetValue(100);
    SetArmourType(A_HELMET);
    SetProtection(BLUNT, 40);
    SetProtection(BLADE, 3);
    SetProtection(KNIFE, 10);
    SetProtection(HEAT, 30);
}
