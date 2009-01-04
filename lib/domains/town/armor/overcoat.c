#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("overcoat");
    SetId(({"coat","overcoat"}));
    SetAdjectives(({"heavy","woolen"}));
    SetShort("a heavy woolen overcoat");
    SetLong("This is a large overcoat, of fine craftsmanship. It's "+
            "likely excellent protection against the cold.");
    SetMass(60);
    SetBaseCost("silver",300);
    SetProtection(BLUNT,10);
    SetProtection(BLADE,10);
    SetProtection(KNIFE,10);
    SetProtection(COLD,5);
    SetArmorType(A_CLOAK);
}

void init(){
    ::init();
}
