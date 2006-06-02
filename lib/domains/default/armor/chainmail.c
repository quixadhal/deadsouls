#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("chainmail shirt");
    SetId(({"armor","chainmail","mail","shirt","chain mail"}));
    SetAdjectives(({"chainmail","sturdy","sturdy-looking","chain mail"}));
    SetShort("chainmail armor");
    SetLong("A body-length suit of armor made of small metal rings fashioned together.");
    SetMass(600);
    SetBaseCost("silver",1000);
    SetDamagePoints(100);
    SetProtection(BLUNT,4);
    SetProtection(BLADE,25);
    SetProtection(KNIFE,25);
    SetArmorType(A_ARMOR);
}
void init(){
    ::init();
}
