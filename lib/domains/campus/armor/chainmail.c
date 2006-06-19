#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("chainmail shirt");
    SetId(({"armor","chainmail armor","chain mail armor","shirt","chainmail shirt","chainmail","chain mail shirt"}));
    SetShort("a sturdy-looking chainmail shirt");
    SetLong("This is a shirt made of metal rings fashioned together as armor. ");
    SetMass(75);
    SetDollarCost(100);
    SetDamagePoints(100);
    SetProtection(BLUNT,4);
    SetProtection(BLADE,25);
    SetProtection(KNIFE,25);
    SetArmorType(A_BODY_ARMOR);
}
