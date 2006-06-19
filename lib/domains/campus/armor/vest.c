#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("paisley vest");
    SetId(({"vest"}));
    SetAdjectives(({"paisley","colorful","hippie","hippy","psychedelic"}));
    SetShort("a colorful, paisley vest");
    SetLong("A stupid-looking hippie vest with psychedelic paisley patterns on it.");
    SetMass(5);
    SetDollarCost(1);
    SetDamagePoints(1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_VEST);
}
