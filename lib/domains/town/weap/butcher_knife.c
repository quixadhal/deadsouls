#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("butcher knife");
    SetId( ({ "knife"}));
    SetAdjectives( ({ "sharp","razor sharp","steel","hefty","stainless","butcher"}));
    SetShort("an 8-inch butcher knife");
    SetLong("This is an 8-inch butcher knife. This razor sharp knife has been forged from molded and hammered high-carbon stainless steel. It is solid, hefty, and well-balanced for kitchen work, but probably not as good a weapon as a carving knife might be.");
    SetMass(50);
    SetBaseCost("silver",95);
    SetVendorType(VT_WEAPON);
    SetClass(15);
    SetDamageType(KNIFE);
    SetWeaponType("knife");
}
int GetFancy(){
    return 1;
}
void init(){
    ::init();
}
