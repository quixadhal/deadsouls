#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("riding boot");
    SetId(({"boot"}));
    SetAdjectives(({"long","riding","black","leather","right"}));
    SetShort("a long riding boot");
    SetLong("A long riding boot, made of black leather. It is "+
            "designed for wear on the right foot and leg.");
    SetMass(60);
    SetBaseCost("silver",75);
    SetDamagePoints(5);
    SetProtection(BLUNT,5);
    SetProtection(BLADE,5);
    SetProtection(KNIFE,5);
    SetArmorType(A_LONG_BOOT);
    SetRestrictLimbs(({"right foot","right leg"}));
}
void init(){
    ::init();
}
