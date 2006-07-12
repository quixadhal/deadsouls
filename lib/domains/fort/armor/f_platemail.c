#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("field platemail armor");
    SetId(({"armor","platemail armor","platemail","field armor"}));
    SetShort("a suit of field platemail armor");
    SetLong("This is a large suit composed of chainmail with "+
      "steel plates attached over vital areas. Unlike a suit "+
      "of typical knight's armor, the plates only cover vital areas "+
      "and key vulnerable points. The rest of the body is protected "+
      "by the chainmail. This makes the suit very protective yet "+
      "usable in the field.");
    SetMass(1000);
    SetDollarCost(8000);
    SetDamagePoints(100);
    SetProtection(BLUNT,30);
    SetProtection(BLADE,40);
    SetProtection(KNIFE,40);
    SetArmorType(A_BODY_ARMOR);
    SetRestrictLimbs(({ "torso", "right leg", "left arm", "left leg", "right arm" }));
}
void init(){
    ::init();
}
