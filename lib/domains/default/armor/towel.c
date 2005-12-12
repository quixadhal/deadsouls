#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("damp towel");
    SetId(({"towel"}));
    SetAdjectives(({"damp","bath","ordinary", "bathtowel"}));
    SetShort("a damp towel");
    SetLong("This is an ordinary bath towel.");
    SetMass(25);
    SetDamagePoints(10);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_PANTS);
}
