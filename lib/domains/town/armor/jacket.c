#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("leather jacket");
    SetId(({"jacket"}));
    SetAdjectives(({"black","leather"}));
    SetShort("a black leather jacket");
    SetLong("This is a black jacket made of leather. ");
    SetMass(50);
    SetBaseCost("silver",100);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,2);
    SetProtection(KNIFE,5);
    SetArmorType(A_ARMOR);
}

void init(){
    ::init();
}
