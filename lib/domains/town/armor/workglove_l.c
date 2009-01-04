#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ARMOR;

static void create() {
    armor::create();
    SetKeyName("workglove");
    SetAdjectives( ({"work", "brown", "leather", "left"}) );
    SetId( ({ "glove" }) );
    SetShort("a leather work glove");
    SetLong("A thick, brown leather work glove, designed for wear on the left hand.");
    SetVendorType(VT_ARMOR);
    SetMass(10);
    SetBaseCost("silver",7);
    SetArmorType(A_GLOVE);
    SetProtection(BLUNT, 1);
    SetProtection(BLADE, 1);
    SetProtection(KNIFE, 2);
    SetProtection(HEAT, 3);
    SetProtection(COLD, 7);
    SetFingers(5);
    SetRestrictLimbs(({"left hand"}));
}

void init(){
    ::init();
}
