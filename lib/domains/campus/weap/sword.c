/*    /domains/Examples/weapon/sword.c
 *    from the Dead Souls LPC Library
 *    a simple sword example, nothing fancy
 *    created by Descartes of Borg 950402
 */

#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("short sword");
    SetId( ({ "sword", "short sword" }) );
    SetAdjectives( ({ "short" }) );
    SetShort("a short sword");
    SetLong("A cheap and rather dull short sword.");
    SetMass(150);
    SetDollarCost(50);
    SetVendorType(VT_WEAPON);
    SetClass(20);
    SetDamageType(BLADE);
    SetWeaponType("blade");
}
