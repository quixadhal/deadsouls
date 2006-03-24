#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("leather jacket");
    SetId(({"jacket"}));
    SetAdjectives(({"strong","leather","black","tough"}));
    SetShort("a black leather jacket");
    SetLong("This is a jacket made of strong, tough leather. ");
    SetMass(150);
    SetBaseCost("silver",100);
    SetDamagePoints(10);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,5);
    SetProtection(KNIFE,5);
    SetArmorType(A_ARMOR);
}
void init(){
    ::init();
}
