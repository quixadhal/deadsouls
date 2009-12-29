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

varargs int eventWearShield(object who, mixed where);

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

varargs int eventWearShield(object who, mixed where) {
    object env = environment(who);
    if( who->GetRace() != "hobbit" ) {
        who->eventPrint("The shield's handle simply does not fit your hand "
                "right.");
        return 0;
    }
    who->eventPrint("You wear "+GetShort()+".");
    if(env) tell_room(env, who->GetName()+" wears "+GetShort()+".", ({who}));
    return 1;
}
