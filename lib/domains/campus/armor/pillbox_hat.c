#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("pillbox hat");
    SetId(({"hat"}));
    SetAdjectives(({"pillbox"}));
    SetShort("a light blue pillbox hat");
    SetLong("A cute little pillbox hat, light blue in "+
            "color. The inside appears to be lined with leopard "+
            "skin. Looks uncomfortable.");
    SetMass(50);
    SetDollarCost(50);
    SetDamagePoints(10);
    SetArmorType(A_HELMET);
    SetProtection(BLUNT, 2);
    SetProtection(BLADE, 2);
    SetProtection(KNIFE, 2);
}
