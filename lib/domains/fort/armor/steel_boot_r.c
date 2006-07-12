#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("steel boot");
    SetId(({"boot","steel boot"}));
    SetAdjectives( ({"steel","strong","tough","right"}) );
    SetShort("a steel boot");
    SetLong("This is the steel combat boot of a professional soldier. "+
      "It is very heavy and very strong. It is designed for wear on the right foot.");
    SetMass(150);
    SetDollarCost(500);
    SetDamagePoints(20);
    SetProtection(BLUNT,20);
    SetProtection(BLADE,30);
    SetProtection(KNIFE,30);
    SetArmorType(A_BOOT);
    SetRestrictLimbs(({"right foot"}));
}
