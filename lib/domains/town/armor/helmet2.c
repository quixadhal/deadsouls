#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("kevlar helmet");
    SetId(({"helmet","cover","kevlar"}));
    SetAdjectives(({"kevlar","battle","hard"}));
    SetShort("a camouflaged kevlar helmet");
    SetLong("This is a military helmet made of a very tough "+
            "polymer fiber that provides excellent ballistic protection. It bears a "+
            "mottled camouflage design to aid in woodland concealment.");
    SetMass(50);
    SetBaseCost("silver",500);
    SetArmorType(A_HELMET);
    SetProtection(BLUNT, 25);
    SetProtection(BLADE, 25);
    SetProtection(KNIFE, 25);
    SetProtection(PIERCE, 25);
}

void init(){
    ::init();
}
