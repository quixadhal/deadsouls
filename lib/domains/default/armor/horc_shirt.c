#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("shirt");
    SetAdjectives( ({"tee"}) );
    SetId( ({"t-shirt", "t shirt", "tshirt", "tee shirt"}) );
    SetShort("a t-shirt that reads: 'I'M HALF-HUMAN'");
    SetLong("A generic piece of armor of indeterminate proportions.");
    SetMass(50);
    SetDamagePoints(100);
    SetArmorType(A_SHIRT);
    SetProtection(BLUNT, 20);
    SetProtection(BLADE, 20);
    SetProtection(KNIFE, 20);
}
void init(){
    ::init();
}
