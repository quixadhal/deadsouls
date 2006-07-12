#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("a white smock");
    SetId(({"smock"}));
    SetAdjectives(({"white"}));
    SetShort("a white smock");
    SetLong("A white smock designed to cover one's entire body.");
    SetMass(5);
    SetDollarCost(1);
    SetDamagePoints(1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_BODY_ARMOR);
    SetRestrictLimbs(({ "torso", "right leg", "left arm", "left leg", "right arm" }));
}
void init(){
::init();
}