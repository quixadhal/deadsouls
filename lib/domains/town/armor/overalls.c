#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_WORN_STORAGE;

static void create(){
    ::create();
    SetKeyName("overalls");
    SetId(({"overalls","pants", "trousers"}));
    SetAdjectives(({"worn","abused","pair of","set of"}));
    SetShort("a pair of overalls");
    SetLong("This is a set of worn and abused overall trousers with large pockets "
            "for carrying stuff.");
    SetMass(50);
    SetMaxCarry(300);
    SetBaseCost("silver",1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_CUSTOM);
    SetRestrictLimbs( ({ "right leg","left leg","torso" }) );
}

void init(){
    ::init();
}
