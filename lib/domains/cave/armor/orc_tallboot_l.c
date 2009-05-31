#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("leather boot");
    SetAdjectives( ({"leather", "simple", "orc", "orcish", "protective","leg","left","full leg"}) );
    SetId(({"boot","left boot"}));
    SetShort("a leather boot");
    SetLong("A thick leather full-leg boot of orcish design. Simple, but quite protective. It is designed for the left foot and leg.");
    SetMass(120);
    SetBaseCost("silver", 200);
    SetProtection(BLUNT,5);
    SetProtection(BLADE,10);
    SetProtection(KNIFE,10);
    SetArmorType(A_LONG_BOOT);
    SetRestrictLimbs(({"left foot","left leg"}));
}

void init(){
    ::init();
}
