#include <lib.h>
#include <vendor_types.h>
inherit LIB_ITEM;
private int charge,rechargeable, drainable,drainrate,fullcharge;
private string powertype,celltype;

int Spent(){
    string *tmp;
    if(rechargeable == 1) return 1;
    if(!charge || charge <= 0){
        charge = 0;
        SetShort("a spent power cell");
        tmp = GetAdjectives();
        tmp += ({"spent", "empty", "drained", "used"});
        SetAdjectives(tmp);
    }
    return 1;
}


void create(){
    ::create();
    AddSave( ({ "charge" }) );
    SetKeyName("cell");
    SetId(({"battery"}));
    SetAdjectives(({"power","generic"}));
    SetShort("a generic power cell");
    SetLong("This is a generic power cell.");
    SetMass(1);
    SetBaseCost(4);
    SetVendorType(VT_TREASURE);
    rechargeable=0;
    drainable = 1;
    drainrate = 1;
    Spent();
}

void init(){
    ::init();
    if(living(environment(this_object())) ) this_object()->eventUse(0);
    if( charge < 200 ) SetBaseCost(0);
    Spent();
}

int SetPowerType(string str){powertype=str; return 1; }
int SetCellType(string str){celltype=str; return 1; }
int SetCharge(int i){charge = i; fullcharge = i; return 1; }
int GetCharge(){ return charge; }
string GetPowerType(){ return powertype; }
string GetCellType(){ return celltype; }
int SetRechargeable(int i){ rechargeable = i; return i;}
int GetRechargeable(){ return rechargeable;}
int SetDrainable(int i){ drainable = i; return i; }
int GetDrainable(){ return drainable; }
int SetDrainRate(int i){ drainrate = i; return i; }
int GetDrainRate(){ return drainrate; }


int ModCharge(int i){
    if(!charge) charge = 0;
    if(i) charge += i;
    return charge;
}

int eventDrain(int i){
    if( drainable != 1 ) return 0;
    if(!charge) charge = 0;
    if(charge && charge > 0) charge -= i;
    if(charge && charge < 0) Spent();
    if(!charge) Spent();
    if( charge < 200 ) SetBaseCost(0);
    return charge;
}

int eventCharge(int i){
    if(!charge) charge =0;
    if(i) charge += i;
    return charge;
}

int eventUse(int i){
    if(query_verb() == "use"){
        write("That's not how it works.");
        return 1;
    }
    if(i == 0){
        set_heart_beat(0);
        return 1;
    }
    if(!drainable) drainable = 1;
    if(drainable && drainable == 0) drainrate = 0;
    else if(!drainable) drainable = 1;
    if( !i || !intp(i) ) i = drainrate;
    set_heart_beat(i);
}

void heart_beat(){
    eventDrain(GetDrainRate() || 1);
}
