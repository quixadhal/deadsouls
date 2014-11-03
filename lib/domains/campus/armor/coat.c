#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("lab coat");
    SetId(({"coat","labcoat"}));
    SetAdjectives(({"lab","science","white","long"}));
    SetShort("a lab coat");
    SetLong("This is a long white lab coat of the type used by scientists "+
            "to avoid getting experimental results on themselves.");
    SetMass(75);
    SetDollarCost(20);
    SetArmorType(A_CLOAK);
}
void init(){
    ::init();
}
