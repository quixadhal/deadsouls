#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create(){
    item::create();
    SetKeyName("cue");
    SetId( ({ "stick", "pole" }) );
    SetAdjectives( ({ "wood","wooden", "pool", "billiards", "billiard"}) );
    SetShort("a billiards cue");
    SetLong("This is a long stick of highly polished wood, used "
            "in the game of billiards.");
    SetVendorType(VT_WEAPON);
    SetDamagePoints(50);
    SetClass(10);
    SetBaseCost("silver",150);
    SetMass(80);
    SetWeaponType("blunt");
    SetHands(2);
    SetDamageType(BLUNT);
}
void init(){
    ::init();
}
