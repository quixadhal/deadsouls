#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("canvas shorts");
    SetId( ({"pants", "shorts"}) );
    SetAdjectives( ({"cloth", "simple"}) );
    SetShort("a pair of canvas shorts");
    SetLong("These appear to be the leasy amount of clothing one can wear and still be modest. If one works in an extremely hot environment, these would probably be great, as they are breathable and yet quite protective of what little they cover.");
    SetMass(50);
    SetArmorType(A_PANTS);
    SetProtection(KNIFE, 5);
    SetProtection(COLD, 5);
}
void init(){
    ::init();
}