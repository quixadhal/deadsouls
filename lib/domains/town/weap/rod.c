#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;
static void create(){
    item::create();
    SetKeyName("rod");
    SetId( ({ "staff", "stick", "pole" , "rod of asclepius", "rod of aesculapius"}) );
    SetAdjectives( ({ "wood","wooden" }) );
    SetShort("a wooden rod with a carved snake on it");
    SetLong("This handsome staff has been carved to "
            "seem to have a snake wrapped around it.");
    SetVendorType(VT_WEAPON);
    SetDamagePoints(50);
    SetClass(100);
    SetBaseCost("silver",250);
    SetMass(100);
    SetWeaponType("blunt");
    SetHands(2);
    SetDamageType(BLUNT);
}
