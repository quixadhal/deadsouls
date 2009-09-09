#include <lib.h>
#include <modules.h>
#include <armor_types.h>
#include <damage_types.h>

inherit LIB_ARMOR;
//bane provides protection from germs and parasites
inherit LIB_BANE;
//the modules below provide qcs functionality
inherit MODULES_ARMOR;
inherit MODULES_CREATE;
inherit MODULES_MAPPING;
inherit MODULES_GENERIC;
inherit MODULES_ROOM;
inherit MODULES_FILE;
inherit MODULES_MONEY;
inherit MODULES_READ;
inherit MODULES_DOOR;

static void create(){
    armor::create();
    SetKeyName("glasses");
    //the tanstaafl id enables this object to function like a
    //qcs staff
    SetId(({"tanstaafl"}));
    SetAdjectives(({"wire", "rimmed"}));
    SetShort("wire-rimmed glasses");
    SetLong("A pair of glasses with magic properties for Creators.");
    SetMass(5);
    SetDamagePoints(1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_VISOR);
    //full protection from all diseases
    SetBane(({"all"}));
}
void init(){
    ::init();
}
