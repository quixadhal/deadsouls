#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("workboot");
    SetAdjectives( ({"strong", "brown", "leather", "left", "work", "heavy"}) );
    SetId( ({"boot"}) );
    SetShort("a brown leather workboot");
    SetLong("A heavy, brown leather workboot with strong ankle and sole support. It is designed for wear on the left foot.");
    SetMass(40);
    SetBaseCost("silver",25);
    SetProtection(BLUNT,5);
    SetProtection(BLADE,5);
    SetProtection(KNIFE,5);
    SetArmorType(A_BOOT);
    SetRestrictLimbs(({"left foot"}));
}

void init(){
    ::init();
}
