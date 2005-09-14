#include <lib.h>
#include <armour_types.h>
#include <damage_types.h>
inherit LIB_ARMOUR;
static void create(){
    armour::create();
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
    SetArmourType(A_PANTS);
}
