#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_WORN_STORAGE;

static void create(){
    ::create();
    SetKeyName("khakis");
    SetAdjectives( ({"khaki", "pair of", "cargo"}) );
    SetId( ({"pants"}) );
    SetShort("a pair of khaki pants");
    SetLong("Khaki pants with large cargo pockets you can put things in.");
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
