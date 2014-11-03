/*    /lib/currency.c
 *    from the Dead Souls LPC Library
 *    handles player bank accounts and carried currency
 *    created by Descartes of Borg 950324
 */


#include <lib.h>

private mapping Currency = ([]);
private mapping Bank     = ([]);

// abstract methods
int CanCarry(int amount);
// end abstract methods

/* ***************  /lib/currency.c driver applies  *************** */

static void create(){
}

/* ***************  /lib/currency.c data manipulation funcs  *************** */

int AddCurrency(string type, int amount){ 
    if( amount > 0 ){
        int curr_mass;
        curr_mass = to_int(currency_mass(amount, type));
        if(curr_mass < 1 ) curr_mass = 1;
        if( !CanCarry(curr_mass) ){
            return -1;
        }
    }
    if( amount + Currency[type] < 0 ){
        return -1;
    }
    return (Currency[type] += amount);
}

int GetCurrency(string type){ return Currency[type]; }
mapping GetCurrencyMap(){ return copy(Currency); }

varargs int GetCurrencyMass(string type){
    int total;

    if( stringp(type) ){
        return currency_mass(Currency[type], type);
    }
    foreach(string currency, int amount in Currency){
        total += currency_mass(amount, currency);
    }
    return total;
}

int AddBank(string bank, string type, int amount){
    if( !stringp(bank) ) error("Bad argument 1 to AddBank().");
    if( !Bank[bank] ){
        if( amount < 1 ) return -1;
        Bank[bank] = ([ "open" : time(), type : amount, "last time" : time(),
                "last trans" : "opened account", 
                "audit" : identify(previous_object(-1)) ]);
        return amount;
    }
    if( Bank[bank][type] + amount < 0 ) return -1;
    Bank[bank][type] += amount;
    Bank[bank]["last time"] = time();
    if( amount > 0 ) Bank[bank]["last trans"] = "deposit";
    else Bank[bank]["last trans"] = "withdrawal";
    Bank[bank]["audit"] = identify(previous_object(-1));
    return Bank[bank][type];
}

int GetBank(string bank, string type){
    if( !Bank[bank] ) return -1;
    else return Bank[bank][type];
}

mapping GetAccountInfo(string bank){
    if( !Bank[bank] ) return 0;
    else return copy(Bank[bank]);
}

varargs int GetNetWorth(string benjamins){
    string curr;
    float net_worth = 0.0;
    int amt;
    foreach(curr, amt in Currency){
        if(valid_currency(curr))
            net_worth += amt * currency_rate(curr);

    }
    foreach(string bank, mapping balance in Bank){
        foreach(curr, amt in balance){
            float tmp;

            switch(curr){
                case "last trans": case "last time": case "audit": case "open":
                    break;
                default:
                    if( (tmp = currency_rate(curr)) < 1 ) break;
                    net_worth += amt * tmp;
                    break;
            }
        }
    }
    if(!benjamins || benjamins == ""||!stringp(benjamins)) benjamins = "gold";
    if(member_array(benjamins,mud_currencies()) == -1) benjamins = "gold";
    if(benjamins && net_worth) return to_int(net_worth / currency_rate(benjamins));
    else return to_int(0);
}

string array GetCurrencies(){ return keys(Currency); }

void ResetBank(){ Bank = ([]); }

void ResetCurrency(){ Currency = ([]); }
