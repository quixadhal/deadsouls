#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("sock");
    SetId(({"sock"}));
    SetAdjectives(({"black","nylon","knee-length","knee","knee length"}));
    SetShort("a long black nylon sock");
    SetLong("A long, knee-length sock, made of black nylon.");
    SetMass(5);
    SetBaseCost("silver",1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_LONG_SOCK);
}

void init(){
    ::init();
}
