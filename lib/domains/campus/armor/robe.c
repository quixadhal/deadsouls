#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("wizard's robe");
    SetId(({"robe","cloak","duster"}));
    SetAdjectives(({"wizard","wizard's","soft"}));
    SetShort("a robe");
    SetLong("This is a large, comfortable-looking robe, like a wizard might wear. ");
    SetMass(75);
    SetDollarCost(100);
    SetDamagePoints(100);
    SetProtection(BLUNT,100);
    SetProtection(BLADE,100);
    SetProtection(KNIFE,100);
    SetArmorType(A_CLOAK);
}
void init(){
    ::init();
}
