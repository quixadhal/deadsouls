#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("sandal");
    SetAdjectives( ({"right", "light"}) );
    SetId( ({"right sandal"}) );
    SetShort("a light sandal");
    SetLong("This is a light, comfortable sandal.");
    SetMass(1);
    SetBaseCost("silver", 5);
    SetArmorType(A_BOOT);
    SetRestrictLimbs(({"right foot"}));
}
void init(){
    ::init();
}
