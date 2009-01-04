#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("generic thing");
    SetId( ({"thing","item","thang","dingus"}) );
    SetAdjectives( ({"generic","sample","template"}) );
    SetShort("a generic thing");
    SetLong("This is an object of indeterminate nature and proportions.");
    SetMass(1);
    SetBaseCost("silver",1);
    SetVendorType(VT_TREASURE);
}

void init(){
    ::init();
}
