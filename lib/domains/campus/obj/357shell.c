#include <lib.h>
#include <vendor_types.h>
inherit LIB_SHELL;

void create(){
    shell::create();
    SetKeyName("357shell");
    SetId(({"shell","casing","spent bullet"}));
    SetAdjectives(({".357","spent","magnum","shell"}));
    SetShort("a .357 shell casing");
    SetLong("This is the spent casing of a .357 magnum bullet.");
    SetCaliber(357);
    SetFirearmType("revolver");
    SetAmmoType("magnum");
    SetVendorType(VT_TREASURE);
}

void init(){
    shell::init();
}
