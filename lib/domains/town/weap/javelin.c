#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("javelin");
    SetAdjectives( ({"throwing", "small", "light"}) );
    SetId( ({"spear"}) );
    SetShort("a javelin");
    SetLong("This is a small, light throwing spear.");
    SetMass(50);
    SetVendorType(VT_WEAPON);
    SetClass(30);
    SetDamageType(PIERCE);
    SetWeaponType("projectile");
    SetBaseCost("silver", 85);
}

void init(){
    ::init();
    add_action("podcast","podcast");
}

int podcast(){
    write("Somehow the javelin just doesn't seem to do that now.");
    return 1;
}
