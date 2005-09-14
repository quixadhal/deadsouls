#include <lib.h>
#include <armour_types.h>
#include <damage_types.h>
inherit LIB_ARMOUR;
static void create(){
    armour::create();
    SetKeyName("wizard's robe");
    SetId(({"robe","cloak","duster"}));
    SetAdjectives(({"wizard","wizard's","soft"}));
    SetShort("a robe");
    SetLong("This is a large, comfortable-looking robe, like a wizard might wear. ");
    SetMass(200);
    SetDamagePoints(100);
    SetProtection(BLUNT,100);
    SetProtection(BLADE,100);
    SetProtection(KNIFE,100);
    SetArmourType(A_CLOAK);
}
