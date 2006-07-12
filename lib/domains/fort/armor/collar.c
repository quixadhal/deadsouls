#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
inherit LIB_BANE;

static void create(){
    armor::create();
    bane::create();
    SetKeyName("collar");
    SetId(({"repellent_collar","bane"}));
    SetAdjectives( ({"repellent","odd","plastic","white"}) );
    SetShort("a plastic collar");
    SetLong("This is a simple collar made of a soft white material. It "+
      "seems to be coated with a fine, clear resin. There is something "+
      "written on the collar.");
    SetRead("Jones the Cat");
    SetMass(1);
    SetDollarCost(200);
    SetDamagePoints(100);
    SetArmorType(A_AMULET);
    SetBane(({"all"}));
}
