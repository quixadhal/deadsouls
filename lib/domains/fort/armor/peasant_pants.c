#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("pants");
    SetId( ({"legwear"}) );
    SetAdjectives( ({"cloth", "simple"}) );
    SetShort("a pair of pants");
    SetLong("A simple pair of pants made of some strong, thick cloth.");
    SetMass(50);
    SetArmorType(A_PANTS);
    SetProtection(KNIFE, 5);
    SetProtection(COLD, 5);
}
void init(){
    ::init();
}
