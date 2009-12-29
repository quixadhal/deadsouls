#include <lib.h>
#include <vendor_types.h>
inherit "/lib/pistol";

int AddClip();
void create(){
    pistol::create();
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
    SetDollarCost(3000);
    SetVendorType(VT_WEAPON);
    AddClip();
    SetLoaded(1);
    SetAmmo(15);
    SetMag(1);
}
int AddClip(){
    if(!present("clip",this_object())){
        new("/domains/campus/obj/9mmclip")->eventMove(this_object());
    }
    return 1;
}
void init(){
    ::init();
}
