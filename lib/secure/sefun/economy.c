#include <daemons.h>

float currency_rate(string type) { 
    return (float)ECONOMY_D->__Query(type, "rate");
}

float currency_inflation(string type) {
    return (float)ECONOMY_D->__Query(type, "inflation");
}

int currency_mass(int x, string type) {
    return to_int((float)ECONOMY_D->__Query(type, "weight") * x);
}

int currency_value(int x, string str) {
    return to_int(x * currency_rate(str));
}

string *mud_currencies() { return ECONOMY_D->__QueryCurrencies(); }

int valid_currency(string str){
    if(member_array(str,mud_currencies()) != -1) return 1;
    else return 0;
}

string query_base_currency(){
    string *monies;
    string least_valuable;
    int base;
    monies = ECONOMY_D->__QueryCurrencies();
    foreach(string currency in monies){
        if(!base) base = ECONOMY_D->__Query(currency,"rate");
        if(ECONOMY_D->__Query(currency,"rate") <= base){
            base = ECONOMY_D->__Query(currency,"rate");
            least_valuable = currency;
        }
    }
    return least_valuable;
}

int query_base_rate(){
    return ECONOMY_D->__Query(query_base_currency(),"rate");
}

int query_player_money(object who) {
    string *monies;
    int base, x;
    x = 0;
    monies = ECONOMY_D->__QueryCurrencies();
    base = ECONOMY_D->__Query(query_base_currency(),"rate");
    foreach(string currency in monies){
        float rate;
        rate = base/ECONOMY_D->__Query(currency,"rate");
        x += who->GetCurrency(currency) * rate;
    }
    return x;
}

int query_base_value(string currency, int amount){
    float rate;
    rate = ECONOMY_D->__Query(currency,"rate");
    return amount * rate;
}

int query_value(int amount,string from_currency, string to_currency){
    int rate, baseval;
    rate = ECONOMY_D->__Query(to_currency,"rate");
    baseval = query_base_value( from_currency, amount );
    return baseval / rate;
}


