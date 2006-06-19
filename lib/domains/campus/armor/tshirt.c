#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("a Def Leppard shirt");
    SetId(({"shirt"}));
    SetAdjectives(({"Def Leppard", "def leppard", "stupid", "black"}));
    SetShort("a Def Leppard shirt");
    SetLong("A stupid-looking black shirt with a ridiculous band logo on it.");
    SetMass(5);
    SetDollarCost(1);
    SetDamagePoints(1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_SHIRT);
}
