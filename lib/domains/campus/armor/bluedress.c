#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("blue dress");
    SetId(({"dress","outfit","costume"}));
    SetAdjectives(({"blue","cotton","lovely","light"}));
    SetShort("a lovely blue dress");
    SetLong("This is a lovely little blue dress of simple, "+
            "efficient design, yet obviously comfortable and practical. The hem is at "+
            "the knees, and the fabric feels soft and light yet strong, like some sort of cotton.");
    SetMass(10);
    SetDollarCost(100);
    SetDamagePoints(100);
    SetArmorType(A_BODY_ARMOR);
}
