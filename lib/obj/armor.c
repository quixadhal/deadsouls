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
    SetArmorType(A_SHIRT);
    SetProtection(KNIFE, 5);
    SetProtection(COLD, 5);
}
void init(){
    ::init();
}
