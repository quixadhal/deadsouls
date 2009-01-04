#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_WORN_STORAGE;

static void create(){
    ::create();
    SetKeyName("wizard robe");
    SetAdjectives( ({"wizards", "wizard's", "soft", "wizard"}) );
    SetId(({"robe","cloak","duster"}));
    SetShort("a robe");
    SetLong("This is a large, comfortable-looking robe, like a wizard might wear. It has many pockets and compartments for holding things.");
    SetProperties(([
                "gamma" : 3,
                "no steal" : 1,
                ]));
    SetCanClose(1);
    SetMaxCarry(5000);
    SetMass(200);
    SetBaseCost("silver",100);
    SetDamagePoints(100);
    SetArmorType(A_CLOAK);
    SetProtection(MAGIC, 10);
    SetProtection(COLD, 10);
}
void init(){
    ::init();
}
