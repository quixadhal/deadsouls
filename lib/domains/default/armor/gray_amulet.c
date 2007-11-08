// The carrier of this amulet will not be forced to quit
// if they hit their max idle time.

#include <lib.h>
#include <rooms.h>
#include <armor_types.h>
#include <damage_types.h>
inherit LIB_ARMOR;

static void create(){
    armor::create();
    SetKeyName("gray amulet");
    SetId(({"amulet", "gray", "idler_amulet" }));
    SetShort("a gray amulet");
    SetLong("This dull, heavy amulet seems rather worthless, though "
      "something about it seems to suggest hidden purpose.");
    SetProperties(([
        "no steal" : 1,
      ]));
    SetMass(100);
    SetBaseCost(2);
    SetArmorType(A_AMULET);
    SetRetainOnDeath(1);
    SetRestrictLimbs( ({ "torso" }) );
}

void init(){
    ::init();
}
