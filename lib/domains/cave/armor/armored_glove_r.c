#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("armored glove");
    SetAdjectives( ({"armored", "orc", "orcish", "protective"}) );
    SetId(({"glove","right glove"}));
    SetShort("an armored glove");
    SetLong("An armored glove of orcish design. It is designed to cover the right hand and arm, and looks extremely durable and protective.");
    SetMass(60);
    SetBaseCost("silver", 300);
    SetProtection(BLUNT,15);
    SetProtection(BLADE,15);
    SetProtection(KNIFE,15);
    SetArmorType(A_LONG_GLOVE);
    SetFingers(6);
    SetRestrictLimbs(({"right hand","right arm"}));
}

void init(){
    ::init();
}
