#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("dress");
    SetId(({"dress"}));
    SetAdjectives(({"improvised","simple"}));
    SetShort("a simple dress");
    SetLong("This is a simple little dress "+
            "made apparently from scrap materials and of improvised design.");
    SetMass(10);
    SetBaseCost("silver",1);
    SetArmorType(A_BODY_ARMOR);
}

void init(){
    ::init();
}
