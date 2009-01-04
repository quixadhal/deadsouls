#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("leather vest");
    SetAdjectives( ({"vest", "leather", "sturdy", "protective"}) );
    SetId(({"vest"}));
    SetShort("a leather vest");
    SetLong("A sturdy leather vest that provides protection and warmth for the torso, while permitting a free range of motion for one's arms.");
    SetMass(50);
    SetBaseCost("silver", 100);
    SetArmorType(A_VEST);
    SetProtection(COLD, 10);
    SetProtection(BLUNT, 1);
    SetProtection(BLADE, 5);
    SetProtection(KNIFE, 5);
}

void init(){
    ::init();
}
