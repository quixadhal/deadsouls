/*    /domains/Ylsrim/armor/shield.c
 *    from the Dead Souls Mud Library
 *    an example shield
 *    created by Lassondra@Dead Souls
 */

#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOR;

int eventWearShield(object who);

static void create() {
    armor::create();
    SetKeyName("wooden shield");
    SetId( ({ "shield", "wooden shield" }) );
    SetShort("a wooden shield");
    SetLong("This shield is a simple circle of woode with no markings.");
    SetDamagePoints(600);
    SetVendorType(VT_ARMOR);
    SetMass(400);
    SetValue(75);
    SetArmorType(A_SHIELD);
    SetWear( (: eventWearShield :) );
    SetProtection(BLUNT, 5);
    SetProtection(BLADE, 3);
    SetProtection(KNIFE, 10);
    SetProtection(SHOCK, 30);
}

int eventWearShield(object who) {
    if( (string)who->GetRace() != "hobbit" ) {
        who->eventPrint("The shield's handle simply does not fit your hand "
                "right.");
        return 0;
    }
    else return 1;
}
