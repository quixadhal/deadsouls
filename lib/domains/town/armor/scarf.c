#include <lib.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;


static void create(){
    armor::create();
    SetKeyName("scarf");
    SetId(({"scarf"}));
    SetAdjectives( ({"handsome","tartan"}) );
    SetShort("a handsome tartan scarf");
    SetLong("This a handsome scarf, very comfortable and "+
      "probably expensive as well.");
    SetMass(10);
    SetBaseCost("silver",50);
    SetDamagePoints(1);
    SetArmorType(A_COLLAR);
}
void init(){
    ::init();
}
