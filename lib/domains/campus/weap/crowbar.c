#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create(){
    item::create();
    SetKeyName("crowbar");
    SetId( ({ "rod" }) );
    SetAdjectives( ({ "heavy", "iron"}) );
    SetShort("a crowbar");
    SetLong("This is heavy iron rod with specialized ends for prying "
            "things open.");
    SetVendorType(VT_WEAPON);
    SetClass(20);
    SetBaseCost("silver",150);
    SetMass(100);
    SetWeaponType("blunt");
    SetHands(1);
    SetDamageType(BLUNT);
}

void init(){
    ::init();
}
