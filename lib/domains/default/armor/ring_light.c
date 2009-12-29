#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("ring");
    SetId(({"ring"}));
    SetAdjectives( ({"magic","magical","glowing","light","glowing"}) );
    SetShort("a brilliant, glowing ring");
    SetLong("This magical ring radiates very bright light that "+
            "can illuminate a room.");
    SetRadiantLight(30);
    SetMass(1);
    SetBaseCost("silver",5000);
    SetArmorType(A_RING);
}

void init(){
    ::init();
}
