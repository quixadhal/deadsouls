/*    /domains/Ylsrim/weapon/stick.c
 *    From the Dead Souls Mud Library
 *    An example simple weapon
 *    Created by Descartes of Borg 970101
 */

#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("stick");
    SetId("stick");
    SetAdjectives("wooden");
    SetShort("a wooden stick");
    SetLong("A wooden stick which looks like it might once have been used "
            "as a walking stick, or perhaps as a poor person's club.");
    SetMass(30);
    SetValue(10);
    SetVendorType(VT_WEAPON);
    SetClass(10);
    SetDamageType(BLUNT);
    SetWeaponType("blunt");
}
