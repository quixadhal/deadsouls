#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("workboot");
    SetAdjectives( ({"left", "work", "leather", "sturdy", "thick"}) );
    SetId( ({"boot"}) );
    SetShort("a leather workboot");
    SetLong("This is a thick, sturdy workboot designed to protect you not only from casual workplace accidents, but also unusually hot environments.");
    SetMass(50);
    SetDollarCost(50);
    SetArmorType(A_BOOT);
    SetProtection(HEAT, 10);
    SetProtection(BLADE, 10);
    SetProtection(BLUNT, 10);
    SetProtection(KNIFE, 10);
    SetRestrictLimbs(({"left foot"}));
}
void init(){
    ::init();
}