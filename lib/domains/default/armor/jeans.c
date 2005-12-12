#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("blue jeans");
    SetId(({"jeans","pants","blue jeans","bluejeans","denim"}));
    SetAdjectives(({"blue","denim","comfortable","broken in","broken-in"}));
    SetShort("a pair of blue jeans");
    SetLong("A comfortable pair of denim blue jeans.");
    SetMass(5);
    SetBaseCost(1);
    SetDamagePoints(1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_PANTS);
}
