#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("filet knife");
    SetAdjectives( ({"filet", "sharp", "razor-sharp", "razor sharp", "balanced", "well-balanced", "well balanced", "high-tempered", "high tempered"}) );
    SetId( ({"knife"}) );
    SetShort("a razor-sharp filet knife");
    SetLong("This is the professional instrument of a serious meat craftsman. It is well-balanced, high-tempered, and of an advanced alloy that is both strong and light, and capable of keeping a wicked edge. This is truly a top of the line kitchen tool.");
    SetMass(50);
    SetVendorType(VT_WEAPON);
    SetClass(35);
    SetDamageType(KNIFE);
    SetWeaponType("knife");
}
void init(){
    ::init();
}