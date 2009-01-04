#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("collar");
    SetId(({"neck guard","guard"}));
    SetAdjectives( ({"neck","collar","chainmail"}) );
    SetShort("a chainmail collar");
    SetLong("This is a simple collar made of chainmail "+
            "to protect the neck from weapons.");
    SetMass(50);
    SetArmorType(A_COLLAR);
    SetProtection(BLUNT,4);
    SetProtection(BLADE,25);
    SetProtection(KNIFE,25);
}

void init(){
    ::init();
}
