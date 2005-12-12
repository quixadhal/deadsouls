#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
inherit "/lib/std/bane";

static void create(){
    armor::create();
    bane::create();
    SetKeyName("collar");
    SetId(({"repellent_collar","bane"}));
    SetAdjectives( ({"repellent","odd","plastic","white"}) );
    SetShort("a plastic collar");
    SetLong("This is a simple collar made of a soft white plastic. It "+
      "seems to be coated with a fine, clear resin. There is something "+
      "written on the collar.");
    SetRead("Disease and parasite repellent collar, test character use only.");
    SetMass(10);
    SetBaseCost("silver",5000);
    SetDamagePoints(100);
    SetArmorType(A_COLLAR);
    SetBane(({"all"}));
}
