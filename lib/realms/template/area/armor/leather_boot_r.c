#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("leather boot");
    SetId(({"boot","leather boot"}));
    SetAdjectives( ({"leather","strong","tough","right"}) );
    SetShort("a leather boot");
    SetLong("This is the leather combat boot of a professional soldier. "+
            "It is very thick and very strong. It is designed for wear on the right foot.");
    SetMass(50);
    SetBaseCost(50);
    SetDamagePoints(20);
    SetProtection(BLUNT,10);
    SetProtection(BLADE,20);
    SetProtection(KNIFE,20);
    SetArmorType(A_BOOT);
    SetRestrictLimbs(({"right foot"}));
}
