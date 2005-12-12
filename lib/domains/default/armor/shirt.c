#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("a white t-shirt");
    SetId(({"shirt","tshirt","t-shirt","t shirt"}));
    SetAdjectives(({"white"}));
    SetShort("a white t-shirt");
    SetLong("An ordinary white t-shirt.");
    SetMass(5);
    SetBaseCost(1);
    SetDamagePoints(1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_SHIRT);
}
