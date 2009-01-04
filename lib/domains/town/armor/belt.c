#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("belt");
    SetId(({"belt"}));
    SetAdjectives(({"leather"}));
    SetShort("a leather belt");
    SetLong("A belt made of leather.");
    SetMass(5);
    SetBaseCost("silver",10);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_BELT);
}

void init(){
    ::init();
}
