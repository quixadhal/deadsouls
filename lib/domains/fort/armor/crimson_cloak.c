#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
static void create(){
    armor::create();
    SetKeyName("crimson cloak");
    SetId(({"cloak","crimson cloak"}));
    SetAdjectives( ({"crimson","proud"}) );
    SetShort("a crimson cloak");
    SetLong("This is the proud, crimson cloak of the Fortress Guards. "+
      "It's made of durable, fine fabric.");
    SetMass(20);
    SetDollarCost(300);
    SetDamagePoints(1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_CLOAK);
}
