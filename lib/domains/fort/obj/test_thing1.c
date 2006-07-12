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
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
}

int TestFun(mixed foo){
    int a = 1;
    if(!objectp(foo)) write("It's not an object.");
    if(!objectp(foo) || (a = 2)) write("a is: "+a); 
    return 1;
}

void init(){
    ::init();
    add_action("TestFun","testfun");
}

