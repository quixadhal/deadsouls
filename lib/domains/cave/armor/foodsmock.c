#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("uniform");
    SetAdjectives( ({"food workers","food worker's"}) );
    SetId( ({"overalls", "smock"}) );
    SetShort("a food worker's uniform");
    SetLong("This is a set of overalls used by food workers to keep their bodies and the food at a healthy distance.");
    SetMass(50);
    SetArmorType(A_BODY_ARMOR);
}
void init(){
    ::init();
}
