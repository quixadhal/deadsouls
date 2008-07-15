#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("paring knife");
    SetId( ({ "knife"}));
    SetAdjectives( ({ "serrated","sharp","razor sharp","steel","hefty","stainless","paring"}));
    SetShort("a 4-inch paring knife");
    SetLong("This is a 4-inch paring knife. This very sharp knife has been forged from molded and hammered high-carbon stainless steel. Though light, it is solid and well-balanced.");
    SetMass(20);
    SetBaseCost("silver",55);
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
