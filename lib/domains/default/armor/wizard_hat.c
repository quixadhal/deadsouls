#include <lib.h>
#include <armour_types.h>
#include <damage_types.h>
inherit LIB_ARMOUR;
static void create(){
    armour::create();
    SetKeyName("wizard hat");
    SetId(({"hat","headgear"}));
    SetAdjectives(({"wizard","wizard's"}));
    SetShort("a wizard's hat");
    SetLong("This is a large, floppy hat with a wide brim all "+
      "around it, and a conical center. It is dark blue in color, "+
      "and is decorated with pictures of yellow moons and stars.");
    SetMass(50);
    SetDamagePoints(100);
    SetArmourType(A_HELMET);
    SetProtection(BLUNT, 20);
    SetProtection(BLADE, 20);
    SetProtection(KNIFE, 20);
}
