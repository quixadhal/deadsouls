#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("smock");
    SetAdjectives( ({"bar workers","food worker's"}) );
    SetId( ({"overalls", "smock", "simple"}) );
    SetShort("a simple smock");
    SetLong("This is a set of overalls used by food workers to keep their bodies and the food at a healthy distance.");
    SetMass(50);
    SetArmorType(A_BODY_ARMOR);
}

void init(){
    ::init();
}
