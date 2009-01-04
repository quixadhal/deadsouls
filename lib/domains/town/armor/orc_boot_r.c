#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("leather boot");
    SetAdjectives( ({"leather", "simple", "orc", "orcish", "inelegant", "protective"}) );
    SetId(({"boot","right boot"}));
    SetShort("a leather boot");
    SetLong("A leather boot of orcish design. Simple and inelegant, but quite protective. It is designed for the right foot.");
    SetMass(60);
    SetBaseCost("silver", 30);
    SetProtection(BLUNT,5);
    SetProtection(BLADE,5);
    SetProtection(KNIFE,5);
    SetArmorType(A_BOOT);
    SetRestrictLimbs(({"right foot"}));
}

void init(){
    ::init();
}
