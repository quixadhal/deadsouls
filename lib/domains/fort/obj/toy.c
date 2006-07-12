#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;

void create(){
    ::create();
    SetKeyName("toy");
    SetId( ({"thing","item","thang","dingus"}) );
    SetAdjectives( ({"generic","sample","template"}) );
    SetShort("a toy");
    SetLong("This is an object of indeterminate nature and proportions.");
    SetItems(([
	({ "three", "four" }) : "beta",
	({ "one", "two" }) : "alpha",
      ]));
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
}
void init(){
    ::init();
}
