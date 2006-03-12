#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("wizards hat");
    SetAdjectives( ({"wizards", "floppy", "large", "conical", "blue"}) );
    SetId( ({"hat"}) );
    SetShort("a wizard's hat");
    SetLong("This is a large, floppy hat with a wide brim all "+
      "around it, and a conical center. It is dark blue in color, "+
      "and is decorated with pictures of yellow moons and stars.");
    SetMass(50);
    SetBaseCost("silver",500);
    SetDamagePoints(100);
    SetArmorType(A_HELMET);
    SetProtection(BLUNT, 20);
    SetProtection(BLADE, 20);
    SetProtection(KNIFE, 20);
}
void init(){
    ::init();
}