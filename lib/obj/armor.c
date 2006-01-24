#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("armor");
    SetId(({"armor"}));
    SetAdjectives(({"generic"}));
    SetShort("generic armor");
    SetLong("A generic piece of armor of indeterminate proportions.");
    SetMass(50);
    SetDamagePoints(100);
    SetArmorType(A_BODY_ARMOR);
    SetProtection(BLUNT, 20);
    SetProtection(BLADE, 20);
    SetProtection(KNIFE, 20);
}
void init(){
    ::init();
}
