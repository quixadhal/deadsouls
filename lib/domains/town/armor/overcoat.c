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
    SetMass(300);
    SetBaseCost("silver",300);
    SetDamagePoints(10);
    SetProtection(BLUNT,10);
    SetProtection(BLADE,10);
    SetProtection(KNIFE,10);
    SetArmorType(A_CLOAK);
}
void init(){
    ::init();
}
