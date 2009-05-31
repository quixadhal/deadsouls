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
    SetMass(2);
    SetBaseCost("silver",1);
    SetArmorType(A_SHIRT);
}

void init(){
    ::init();
}
