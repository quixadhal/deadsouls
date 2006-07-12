#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("steel helmet");
    SetId(({"helmet","steel helmet"}));
    SetAdjectives( ({"round","shiny","strong","light"}) );
    SetShort("a crimson helmet");
    SetLong("This is the round steel helmet of a professional soldier. "+
      "It has been painted crimson in the manner of the elite "+
      "Fortress Praetorian Guard.");
    SetMass(50);
    SetDollarCost(100);
    SetDamagePoints(1000);
    SetProtection(BLUNT,20);
    SetProtection(BLADE,30);
    SetProtection(KNIFE,30);
    SetArmorType(A_HELMET);
}
