#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;


static void create() {
    item::create();
    SetKeyName("carving knife");
    SetId( ({ "knife"}));
    SetAdjectives( ({ "serrated","sharp","razor sharp","steel","hefty","stainless","carving"}));
    SetShort("a serrated, 8-inch carving knife");
    SetLong("This is a serrated, 8-inch carving knife. This razor sharp "+
            "knife has been forged from molded and hammered high-carbon "+
            "stainless steel. It is solid, hefty, and well-balanced. "+
            "This is a dangerous knife, and can be considered a very "+
            "deadly weapon.");
    SetMass(50);
    SetVendorType(VT_WEAPON);
    SetClass(30);
    SetDamageType(KNIFE);
    SetWeaponType("knife");
}
void init(){
    ::init();
}
