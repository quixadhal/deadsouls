#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("shot ball");
    SetAdjectives( ({"ball of", "palm sized", "shot"}) );
    SetId( ({"ball", "lead", "shot", "shotput"}) );
    SetShort("a palm-sized ball of lead");
    SetLong("This is a ball of lead, approximately nine inches in diameter. It is a weapon called a 'shot', intended to be thrown at an enemy.");
    SetMass(50);
    SetVendorType(VT_WEAPON);
    SetClass(30);
    SetDamageType(BLUNT);
    SetWeaponType("projectile");
}
void init(){
    ::init();
}
