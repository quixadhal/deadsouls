#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("platemail armor");
    SetId(({"armor","platemail armor","platemail"}));
    SetAdjectives("suit of");
    SetShort("a full suit of platemail armor");
    SetLong("This is a large, extremely heavy suit composed of "+
      "steel plates attached over most areas. Like a suit "+
      "of knight's armor, the plates cover vital areas "+
      "and key vulnerable points, as well as areas in between. "+
      "This makes the suit very protective, but a bit awkward."); 
    SetMass(1900);
    SetDollarCost(8000);
    SetDamagePoints(100);
    SetProtection(BLUNT,45);
    SetProtection(BLADE,55);
    SetProtection(KNIFE,55);
    SetArmorType(A_BODY_ARMOR);
    SetRestrictLimbs(({ "torso", "right leg", "left arm", "left leg", "right arm" }));
}
