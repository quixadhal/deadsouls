#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
inherit LIB_DONATE;

void create(){
    donate::create();
    item::create();
    SetKeyName("donation box");
    SetId( ({"box","donation box"}) );
    SetAdjectives( ({"small","wood","wooden"}) );
    SetShort("a donation box");
    SetLong("This is a small donation box, made out of wood. "+
            "It is used to donate money, which is then used "+
            "to feed the hungry, clothe the naked, et cetera ad "+
            "nauseam. ");
    SetMass(20);
    SetBaseCost("silver",10);
    SetVendorType(VT_TREASURE);
    SetLocalCurrency("silver");
    SetOwner("The Seventh Day Atheist Congregation");
}

string GetLong(){
    string extra = donate::GetLong();
    string base = item::GetLong();
    return base+extra;
}

void init(){
    ::init();
}

mixed CanGet(){ 
    return "This is for the needy, you scum! Earn your keep!";
}
