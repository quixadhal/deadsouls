#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("sword");
    SetId( ({ "sword" }) );
    SetAdjectives( ({ "fine-looking","fine" }) );
    SetShort("a fine-looking sword");
    SetLong("A finely crafted military sword. It is both "+
      "elegantly designed and well-balanced, "+
      "suggesting it is meant for combat leaders.");
    SetMass(150);
    SetDollarCost(1000);
    SetVendorType(VT_WEAPON);
    SetClass(35);
    SetDamageType(BLADE);
    SetWeaponType("blade");
}
