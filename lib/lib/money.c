#include <lib.h>
mapping Money = ([]);

int AddMoney(string type, int amount){ 
    object pile;
    pile = new(LIB_BLANK_PILE);
    pile->SetPile(type,amount);
    pile->eventMove(this_object());
    Money[type] = amount;
    return 1;
}

varargs void SetMoney(mixed val, int amount){
    if( stringp(val) ) AddMoney(val, amount);
    else if( mapp(val) ){
        string *currs;
        int i;
        i = sizeof(currs = keys(val));
        while(i--){ 
            AddMoney(currs[i], val[currs[i]]);
            Money[currs[i]] = val[currs[i]];
        }
    }
    else error("Bad argument 1 to SetMoney().");
}

mapping GetMoneyMap(){ return copy(Money); }
