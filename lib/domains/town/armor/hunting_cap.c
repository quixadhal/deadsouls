#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("hunting cap");
    SetId(({"hat","headgear","cap"}));
    SetAdjectives(({"soft","felt","hunting"}));
    SetShort("a hunting cap");
    SetLong("This is a soft, felt cap one wears when hunting. "+
            "It has a single feather jauntily stuck to the side.");
    SetMass(10);
    SetBaseCost("silver",50);
    SetDamagePoints(10);
    SetArmorType(A_HELMET);
    SetProtection(BLUNT, 1);
    SetProtection(BLADE, 1);
    SetProtection(KNIFE, 1);
    SetItems( ([
                ({"pheasant","feather"})  : "Looks like a pheasant feather.",
                ]) );
}

void init(){
    ::init();
}
