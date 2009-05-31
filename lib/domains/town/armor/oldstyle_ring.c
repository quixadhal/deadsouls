#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("metal ring");
    SetId(({"ring"}));
    SetAdjectives( ({"simple","metal"}) );
    SetShort("a simple ring");
    SetLong("This is a simple metal ring.");
    SetMass(1);
    SetValue(100);
    SetArmorType(A_RING);
}

void init(){
    ::init();
}
