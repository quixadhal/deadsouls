#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("blue jeans");
    SetId(({"jeans","pants","blue jeans","bluejeans","denim"}));
    SetAdjectives(({"blue","denim","comfortable","broken in","broken-in"}));
    SetShort("a pair of blue jeans");
    SetLong("A comfortable pair of denim blue jeans.");
    SetMass(20);
    SetBaseCost("silver",1);
    SetArmorType(A_PANTS);
}

void init(){
    ::init();
}
