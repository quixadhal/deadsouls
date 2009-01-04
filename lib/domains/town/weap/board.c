#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("board of ed");
    SetId(({"board","board of ed","board of education"}));
    SetAdjectives(({"ed","education"}));
    SetShort("the board of education");
    SetLong("A long, wide wooden board with a handle at one "+
            "end. Inscribed along its length are the words: "+
            "'Board of Education'.");
    SetMass(50);
    SetBaseCost("silver",20);
    SetVendorType(VT_WEAPON);
    SetClass(20);
    SetDamageType(BLUNT);
    SetWeaponType("blunt");
}
