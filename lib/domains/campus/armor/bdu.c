#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("battle dress uniform");
    SetId(({"uniform","bdu","bdu's","fatigues"}));
    SetAdjectives(({"camouflage","battle","dress","combat"}));
    SetShort("a battle dress uniform");
    SetLong("This uniform consists of a camouflage blouse and "+
            "pants. It is green with mottled brown and black camouflage patterns, "+
            "and probably provides excellent concealment in a wooded environment.");
    SetMass(10);
    SetDollarCost(100);
    SetDamagePoints(100);
    SetArmorType(A_BODY_ARMOR);
}
