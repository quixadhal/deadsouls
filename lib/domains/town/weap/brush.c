#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create(){
    item::create();
    SetKeyName("brush");
    SetId( ({ "brush"  }) );
    SetAdjectives( ({ "bath","shower","wood","wooden" }) );
    SetShort("a shower brush");
    SetLong("This is a long wooden brush with soft bristles "+
            "at the end, designed to assist in reaching "+
            "tough-to-scrub areas while bathing.");
    SetVendorType(VT_WEAPON);
    SetClass(10);
    SetBaseCost("silver",25);
    SetMass(100);
    SetWeaponType("blunt");
    SetDamageType(BLUNT);
}
void init(){
    ::init();
}
