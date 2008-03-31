#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_BURN;


void create(){
    item::create();
    burn::create();
    SetKeyName("magic pipe");
    SetId( ({"pipe","smoking pipe","magical pipe","pipe of insight"}) );
    SetAdjectives( ({"magic","magical","khadgar's"}) );
    SetShort("a smoking pipe");
    SetLong("This is a handsome pipe for smoking, made of ivory.");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetRadiantLight(7);
    SetFuelRequired(1);
    SetMaxFuel(100);
    SetFuelAmount(100);
    SetRefuelable(1);
}
int GetRadiantLight(int ambient) {
    if( !GetLit() ) return 0;
    else return item::GetRadiantLight(ambient);
}
string GetShort() {
    return item::GetShort() + burn::GetShort();
}
varargs string GetLong(string unused) {
    string tmp;

    tmp = burn::GetLong(unused);
    if( tmp != "" ) return item::GetLong(unused) + "\n" + tmp;
    else return item::GetLong(unused);
}
void init(){
    ::init();
}
