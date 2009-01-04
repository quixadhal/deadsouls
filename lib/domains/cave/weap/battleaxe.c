#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("battle axe");
    SetId(({"axe","battleaxe"}));
    SetAdjectives( ({ "huge","large","heavy","montrously","battle",
                "two handed" }) ); 
    SetShort("a huge battle axe");
    SetLong("A monstrously large and heavy battle axe.");
    SetMass(1000);
    SetHands(2);
    SetBaseCost("silver",2500);
    SetVendorType(VT_WEAPON);
    SetClass(50);
    SetDamageType(BLADE|BLUNT);
    SetWeaponType("blade");
}

void init(){
    ::init();
}
