#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOR;

static void create() {
    armor::create();
    SetKeyName("leather glove");
    SetId( ({ "glove" }) );
    SetAdjectives( ({ "black","leather" }) );
    SetShort("a long black leather glove");
    SetLong("A long black glove with five fingers.");
    SetVendorType(VT_ARMOR);
    SetMass(10);
    SetBaseCost("silver",25);
    SetArmorType(A_LONG_GLOVE);
    SetProtection(BLUNT, 1);
    SetProtection(BLADE, 1);
    SetProtection(KNIFE, 2);
    SetProtection(HEAT, 3);
    SetProtection(COLD, 7);
    SetFingers(5);
}

void init(){
    ::init();
}
