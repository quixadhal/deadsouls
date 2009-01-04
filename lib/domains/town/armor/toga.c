#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("healer's toga");
    SetId(({"toga"}));
    SetAdjectives(({"long","white","comfotable"}));
    SetShort("a long, white toga");
    SetLong("This is a comfortable, full-length toga. It is the sort of "
            "elegant garment that ancient politicians and healers once wore.");
    SetMass(75);
    SetBaseCost("silver",100);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,10);
    SetProtection(KNIFE,10);
    SetArmorType(A_CLOAK);
}

void init(){
    ::init();
}
