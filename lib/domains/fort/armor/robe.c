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
    SetLong("This is a large, comfortable-looking robe, like a wizard might wear. "+
      "It is decorated with images of the moon and shooting stars.");
    SetMass(100);
    SetDollarCost(500);
    SetDamagePoints(50);
    SetProtection(BLUNT,10);
    SetProtection(BLADE,10);
    SetProtection(KNIFE,10);
    SetArmorType(A_CLOAK);
}
