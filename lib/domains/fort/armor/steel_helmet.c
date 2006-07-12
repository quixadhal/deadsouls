#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("steel helmet");
    SetId(({"helmet","steel helmet"}));
    SetAdjectives( ({"round","shiny","strong","light"}) );
    SetShort("a steel helmet");
    SetLong("This is the round steel helmet of a professional soldier. "+
      "It is very strong and relatively light.");
    SetMass(50);
    SetDollarCost(100);
    SetDamagePoints(1000);
    SetProtection(BLUNT,20);
    SetProtection(BLADE,30);
    SetProtection(KNIFE,30);
    SetArmorType(A_HELMET);
}
