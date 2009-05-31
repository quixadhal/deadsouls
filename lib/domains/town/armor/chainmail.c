#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("chainmail shirt");
    SetId(({"armor","chainmail","mail","shirt","chain mail"}));
    SetAdjectives(({"chainmail","sturdy","sturdy-looking","chain mail"}));
    SetShort("a sturdy-looking chainmail shirt");
    SetLong("This is a shirt made of small, thin metal rings fashioned together as armor. ");
    SetMass(200);
    SetBaseCost("silver",800);
    SetProtection(BLUNT,5);
    SetProtection(BLADE,20);
    SetProtection(KNIFE,20);
    SetArmorType(A_ARMOR);
}

void init(){
    ::init();
}
