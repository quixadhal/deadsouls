#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("steel helmet");
    SetId(({"helmet","cover","steel"}));
    SetAdjectives(({"steel","battle","hard"}));
    SetShort("a steel helmet");
    SetLong("This is a helmet made of tough  metal.");
    SetMass(200);
    SetBaseCost("silver",500);
    SetArmorType(A_HELMET);
    SetProtection(BLUNT, 20);
    SetProtection(BLADE, 20);
    SetProtection(KNIFE, 20);
}

void init(){
    ::init();
}
