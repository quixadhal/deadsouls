#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("bandanna");
    SetAdjectives( ({"red"}) );
    SetId( ({"kerchief", "handkerchief"}) );
    SetShort("a red bandanna");
    SetLong("This is a red kerchief fashioned into improvised headgear, possibly to prevent getting dirt on one's hair and vice versa.");
    SetMass(1);
    SetBaseCost("silver",1);
    SetArmorType(A_HELMET);
    SetProtection(BLUNT, 1);
    SetProtection(BLADE, 1);
    SetProtection(KNIFE, 1);
}

void init(){
    ::init();
}
