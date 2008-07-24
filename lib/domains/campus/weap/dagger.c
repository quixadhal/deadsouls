/*    /domains/Examples/weapon/dagger.c
 *    from the Dead Souls LPC Library
 *    a simple dagger example, nothing fancy
 *    created by Descartes of Borg 950402
 */

#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("sharp dagger");
    SetId( ({ "dagger"}));
    SetAdjectives( ({ "sharp","fine"}));
    SetShort("a sharp dagger");
    SetLong("A dagger.");
    SetMass(150);
    SetDollarCost(50);
    SetVendorType(VT_WEAPON);
    SetClass(150);
    SetDamageType(KNIFE);
    SetWeaponType("knife");
}
