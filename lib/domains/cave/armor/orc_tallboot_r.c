#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("leather boot");
    SetAdjectives( ({"leather", "simple", "orc", "orcish", "protective","leg","right","full leg"}) );
    SetId(({"boot","right boot"}));
    SetShort("a leather boot");
    SetLong("A thick leather full-leg boot of orcish design. Simple, but quite protective. It is designed for the right foot and leg.");
    SetMass(120);
    SetBaseCost("silver", 200);
    SetProtection(BLUNT,5);
    SetProtection(BLADE,10);
    SetProtection(KNIFE,10);
    SetArmorType(A_LONG_BOOT);
    SetRestrictLimbs(({"right foot","right leg"}));
}

void init(){
    ::init();
}
