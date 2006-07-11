/*    /domains/Examples/armor/glove.c
 *    from the Dead Souls LPC Library
 *    an example glove
 *    created by Descartes of Borg 950408
 */

#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOR;

static void create() {
    armor::create();
    SetKeyName("red glove");
    SetId( ({ "glove", "red glove" }) );
    SetAdjectives( ({ "red" }) );
    SetShort("a red glove");
    SetLong("A red glove with five fingers.");
    SetDamagePoints(75);
    SetVendorType(VT_ARMOR);
    SetMass(50);
    SetValue(20);
    SetArmorType(A_GLOVE);
    SetProtection(BLUNT, 1);
    SetProtection(BLADE, 1);
    SetProtection(KNIFE, 2);
    SetProtection(HEAT, 3);
    SetProtection(COLD, 7);
    SetFingers(5);
}
