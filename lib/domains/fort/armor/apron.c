#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("leather apron");
    SetId( ({"apron"}) );
    SetAdjectives(({"white"}));
    SetShort("a thick leather apron");
    SetLong("This is a very tough, thick leather apron. It's just the thing to be wearing if you expect flaming chunks of metal to come flying at you on occasion. It also appears to be excellent protection against the sort of searing heat one could expect in a forge.");
    SetMass(5);
    SetDollarCost(1);
    SetDamagePoints(1);
    SetArmorType(A_ARMOR);
    SetProtection(HEAT, 30);
    SetRestrictLimbs(({ "torso" }));
}
void init(){
    ::init();
}
