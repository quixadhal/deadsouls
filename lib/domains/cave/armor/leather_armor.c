/*    /domains/Ylsrim/armor/artrell_armor.c
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
    SetKeyName("leather armor");
    SetId( ({ "armor", "leather armor" }) );
    SetAdjectives( ({ "leather" }) );
    SetShort("a suit of leather armor");
    SetLong("An ordinary suit of leather armor. It is worn on one's arms, torso and legs.");
    SetVendorType(VT_ARMOR);
    SetMass(750);
    SetValue(150);
    SetDamagePoints(800);
    SetProtection(BLUNT, 20);
    SetProtection(BLADE, 20);
    SetProtection(KNIFE, 25);
    SetProtection(HEAT, 7);
    SetProtection(COLD, 4);
    SetProtection(SHOCK, 15);
    SetArmorType(A_BODY_ARMOR);
}
void init(){
    ::init();
}
