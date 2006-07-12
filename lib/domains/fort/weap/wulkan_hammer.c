#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("hammer");
    SetId( ({ "weapon"}));
    SetAdjectives( ({ "generic" }));
    SetShort("a heavy iron hammer");
    SetLong("This thick, crude instrument is shockingly heavy for its size. Its large iron head seems to have seen a great deal of extreme violence, and the wooden shaft is scored with what must be years of abuse, grease, nicks, and sweat.");
    SetMass(300);
    SetVendorType(VT_WEAPON);
    SetClass(20);
    SetDamageType(BLUNT);
    SetWeaponType("blunt");
}
void init(){
    ::init();
}