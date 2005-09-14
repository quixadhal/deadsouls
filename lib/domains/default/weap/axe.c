/*    /domains/Examples/weapon/axe.c
 *    from the Nightmare IV LPC Library
 *    a simple axe example, nothing fancy
 *    created by Descartes of Borg 950402
 */

#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("sharp axe");
    SetId( ({ "axe"}));
    SetAdjectives( ({ "sharp","small","hand"}));
    SetShort("a hand axe");
    SetLong("A small, sharp hand axe, suitable for close combat.");
    SetMass(150);
    SetVendorType(VT_WEAPON);
    SetClass(15);
    SetDamageType(BLADE);
    SetWeaponType("blade");
}
