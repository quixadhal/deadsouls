#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;
inherit LIB_BANE;

static void create(){
    armor::create();
    SetKeyName("collar");
    SetId(({"repellent_collar","bane"}));
    SetAdjectives( ({"repellent","odd","plastic","white"}) );
    SetShort("a plastic collar");
    SetLong("This is a simple collar made of a soft white plastic. It "+
            "seems to be coated with a fine, clear resin. There is something "+
            "written on the collar.");
    SetProperties(([
                "no steal" : 1,
                ]));
    SetRead("Disease and parasite repellent collar, test character use only.");
    SetMass(10);
    SetBaseCost("silver", 10);
    SetDamagePoints(100);
    SetArmorType(A_COLLAR);
    SetBane(({"all"}));
}
void init(){
    ::init();
}
