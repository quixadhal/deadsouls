#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("paisley vest");
    SetId(({"vest"}));
    SetAdjectives(({"goofy","goofy-looking", "goofy looking", "paisley","colorful","hippie","hippy","psychedelic"}));
    SetShort("a colorful paisley vest");
    SetLong("A goofy-looking hippie vest with psychedelic paisley patterns on it.");
    SetMass(5);
    SetBaseCost("silver",1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_VEST);
}

void init(){
    ::init();
}
