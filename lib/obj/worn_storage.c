#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_WORN_STORAGE;

static void create(){
    ::create();
    SetKeyName("generic worn storage");
    SetId(({"thing","worn_storage","storage","article"}));
    SetAdjectives(({"article of","generic","worn"}));
    SetShort("a generic article of worn storage");
    SetLong("This is a thing that can be worn which can also hold things.");
    SetCanClose(1);
    SetMaxCarry(100);
    SetMass(200);
    SetBaseCost("silver",10);
    SetDamagePoints(100);
    SetArmorType(A_PANTS);
    SetProtection(COLD, 1);
}
void init(){
    ::init();
}
