#include <lib.h>
#include <vendor_types.h>
inherit LIB_TORCH;

void create(){
    ::create();
    SetKeyName("lamp");
    SetId( ({"thing","item","thang","dingus"}) );
    SetAdjectives( ({"generic","sample","template"}) );
    SetShort("a tall lamp");
    SetLong("This is a tall lamp embedded into the floor, designed to provide light.");
    SetPreventGet("The lamp is embedded into the floor.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetRadiantLight(25);
}

void init(){
    ::init();
    if(!GetLit() && CanBurn(this_object())){
        this_object()->eventBurn(0,0,1);
    }
}
