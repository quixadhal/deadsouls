#include <lib.h>
#include <vendor_types.h>
inherit LIB_PISTOL;

void create(){
    ::create();
    SetKeyName("gun");
    SetId(({"gun","pistol"}));
    SetAdjectives(({"9mm","automatic","9 millimeter","semiautomatic"}));
    SetShort("a 9 millimeter semiautomatic pistol");
    SetLong("This is a large, semiautomatic pistol that fires 9mm rounds. It is "+
            "a heavy, serious-looking weapon.");
    SetMillimeter(9);
    SetFirearmType("auto");
    SetAmmoType("acp");
    SetMass(30);
    SetBaseCost("silver",900);
    SetVendorType(VT_WEAPON);
}
void init(){
    ::init();
}
