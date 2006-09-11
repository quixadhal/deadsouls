#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("shirt");
    SetAdjectives( ({"horse", "t", "simple", "cotton"}) );
    SetId( ({"tshirt", "t-shirt"}) );
    SetShort("a shirt");
    SetLong("A simple cotton shirt with a design of a horse drawn on it, and the slogan: 'I'm a little horse girl!'");
    SetMass(5);
    SetBaseCost("silver",1);
    SetDamagePoints(1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_SHIRT);
    //SetRestrictLimbs(({"torso"}));
}
void init(){
    ::init();
}
