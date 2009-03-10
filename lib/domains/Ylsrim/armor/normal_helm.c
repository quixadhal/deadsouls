/*    /domains/Ylsrim/armor/helm.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOR;


static void create() {
    armor::create();
    SetKeyName("wooden helm");
    SetId("helm");
    SetAdjectives("wooden");
    SetShort("a wooden helm");
    SetLong("It is a lame wooden helm.");
    SetDamagePoints(3000);
    SetVendorType(VT_ARMOR);
    SetMass(200);
    SetValue(100);
    SetArmorType(A_HELMET);
    SetProtection(BLUNT, 40);
    SetProtection(BLADE, 3);
    SetProtection(KNIFE, 10);
    SetProtection(HEAT, 30);
}
