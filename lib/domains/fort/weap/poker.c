#include <lib.h>
#include <damage_types.h>
#include <vendor_types.h>

inherit LIB_ITEM;

static void create() {
    item::create();
    SetKeyName("poker");
    SetAdjectives( ({"iron"}) );
    SetId( ({ "weapon"}));
    SetShort("an iron poker");
    SetLong("This tool is most often red-hot, and used to stoke fires. As a weapon it is a nasty piece of work, with a heavy iron shaft and flanged points at the end.");
    SetMass(50);
    SetVendorType(VT_ALL);
    SetClass(15);
    SetDamageType(BLUNT);
    SetWeaponType("blunt");
}
void init(){
    ::init();
}