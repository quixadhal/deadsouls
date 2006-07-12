#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("chainmail shirt");
    SetId(({"armor","chainmail armor","chain mail armor","shirt","chainmail shirt","chainmail","chain mail shirt"}));
    SetShort("a sturdy-looking chainmail shirt");
    SetLong("This is a shirt made of metal rings fashioned together as armor.");
    SetMass(600);
    SetDollarCost(1000);
    SetDamagePoints(100);
    SetProtection(BLUNT,15);
    SetProtection(BLADE,30);
    SetProtection(KNIFE,30);
    SetArmorType(A_ARMOR);
}
