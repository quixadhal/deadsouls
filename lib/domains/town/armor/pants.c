#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("leather pants");
    SetId(({"pants"}));
    SetAdjectives(({"strong","leather","black","tough"}));
    SetShort("leather pants");
    SetLong("These are trousers made of strong, tough leather. ");
    SetMass(150);
    SetBaseCost("silver",110);
    SetProtection(BLADE,10);
    SetProtection(KNIFE,10);
    SetArmorType(A_PANTS);
}

void init(){
    ::init();
}
