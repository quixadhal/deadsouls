#include <lib.h>
#include <vendor_types.h>
#include <damage_types.h>
inherit LIB_FIREARM;
void create(){
    ::create();
    SetKeyName("rifle");
    SetId(({"rifle","m16","M-16","weapon"}));
    SetAdjectives(({"m-16","M-16","m16","M16","assault","colt"}));
    SetShort("an M-16 assault rifle");
    SetLong("This sleek black weapon has been a military workhorse "+
            "for decades. It is durable, accurate, and above all, extraordinarily deadly. "+
            "This rifle uses .223 caliber ammunition magazines.");
    SetCaliber(223);
    SetFirearmType("auto");
    SetFirearmName("rifle");
    SetAmmoType("nato");
    SetMass(60);
    SetDollarCost(1000);
    SetHands(2);
    SetClass(20);
    SetWeaponType("blunt");
    SetDamageType(BLUNT);
    SetVendorType(VT_WEAPON);
}

void init(){
    ::init();
}
