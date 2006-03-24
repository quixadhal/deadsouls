#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("goggles");
    SetId(({"goggles"}));
    SetAdjectives(({"thick","driving","protective","eyewear"}));
    SetShort("driving goggles");
    SetLong("A set of protective goggles for wearing during "+
      "high-speed travel.");
    SetMass(5);
    SetBaseCost("silver",30);
    SetDamagePoints(1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_VISOR);
}
void init(){
    ::init();
}
