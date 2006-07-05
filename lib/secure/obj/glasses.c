#include <lib.h>
#include <modules.h>
#include <armor_types.h>
#include <damage_types.h>

inherit LIB_ARMOR;
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
    SetId(({"tanstaafl"}));
    SetAdjectives(({"wire-rimmed", "wire rimmed"}));
    SetShort("wire-rimmed glasses");
    SetLong("A pair of glasses with magic properties for Creators.");
    SetMass(5);
    SetBaseCost("silver",30);
    SetDamagePoints(1);
    SetProtection(BLUNT,1);
    SetProtection(BLADE,1);
    SetProtection(KNIFE,1);
    SetArmorType(A_VISOR);
}
void init(){
    ::init();
}
