#include <lib.h>

inherit LIB_PILE;

static void create(){
    pile::create();
    SetKeyName("money");
}

string array GetId(){
    string array id;

    id = pile::GetId();
    if( PileType ){
        if(PileType != "dollars"){
            id += ({ PileType, PileAmount + " " + PileType +" coins"});
            return ({ id..., "money", "pile","coins","coin" });
        }
        else {
            id += ({ PileType, PileAmount + " " + PileType +" dollars"});
            return ({ id..., "money", "pile","bill","bills" });
        }
    }
}

string GetShort(){
    string sum;
    if(!PileAmount) sum = "some";
    else sum = cardinal(PileAmount);
    if(PileType != "dollars"){
        return sum + " " + PileType + " coins";
    }
    else return sum + " " + PileType ;
}

