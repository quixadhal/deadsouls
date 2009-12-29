/*    /domains/Ylsrim/armor/helm.c
 *    from the Dead Souls Mud Library
 *    created by Descartes of Borg 960302
 */

#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOR;

varargs int WearHelm(object who, mixed where);

static void create() {
    armor::create();
    SetKeyName("desert helm");
    SetId( ({ "helm", "desert helm" }) );
    SetAdjectives( ({ "sandy", "brown" }) );
    SetShort("a dusty helm");
    SetLong("This helm is the Desert helm, bestowed upon honoured "
            "knights of the desert.  It is made from a magic sand bound "
            "together to protect the heads of the righteous.");
    SetDamagePoints(3000);
    SetVendorType(VT_ARMOR);
    SetMass(200);
    SetValue(300);
    SetArmorType(A_HELMET);
    SetWear( (: WearHelm :) );
    SetProtection(BLUNT, 40);
    SetProtection(BLADE, 3);
    SetProtection(KNIFE, 10);
    SetProtection(HEAT, 30);
}

varargs int WearHelm(object who, mixed where) {
    object env = environment(who);
    if( who->GetMorality() < 300 ) {
        who->eventPrint("The helm burns your head!");
        who->eventReceiveDamage(0, HEAT, random(10) + 10, 0, "head");
        return 0;
    }
    who->eventPrint("You wear the desert helm.");
    if(env) tell_room(env, who->GetName()+" wears the desert helm.", ({who}));
    return 1;
}
