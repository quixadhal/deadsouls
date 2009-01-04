#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("sock");
    SetId(({"sock"}));
    SetAdjectives(({"white","cotton","athletic"}));
    SetShort("a white cotton sock");
    SetLong("An athletic sock, made of white cotton.");
    SetMass(5);
    SetBaseCost("silver",1);
    SetArmorType(A_SOCK);
}

void init(){
    ::init();
}
