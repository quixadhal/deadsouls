/*    /domains/Ylsrim/armour/shield.c
 *    from the Dead Souls V Object Library
 *    an example shield
 *    created by Lassondra@Dead Souls
 */

#include <lib.h>
#include <armour_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOUR;

int eventWearShield(object who);

static void create() {
    armour::create();
    SetKeyName("wooden shield");
    SetId( ({ "shield", "wooden shield" }) );
    SetShort("a wooden shield");
    SetLong("This shield is a simple circle of woode with no markings.");
    SetDamagePoints(600);
    SetVendorType(VT_ARMOUR);
    SetMass(400);
    SetValue(75);
    SetArmourType(A_SHIELD);
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
