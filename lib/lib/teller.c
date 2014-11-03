/*   /lib/teller.c
 *   from the Dead Souls Object Library
 *   npc interface which handles banking transactions
 *   created by Blitz@Dead Souls 960105
 *   Version: @(#) teller.c 1.3@(#)
 *   Last Modified: 96/10/22
 */

#include <lib.h>
#include <daemons.h>
#include "include/teller.h"

inherit LIB_SENTIENT;
inherit LIB_MANYCOINS;

string BankName, LocalCurrency;
string *Currencies;
int LocalFee, NonLocalFee, OpenFee;
float ExchangeFee;

static void create(){
    sentient::create();
    BankName = "Town Trust";
    LocalCurrency = "silver";
    LocalFee = 1;
    NonLocalFee = 5;
    OpenFee = 5;
    Currencies = ({ "copper", "silver", "electrum", "gold", "platinum" });
    SetCommandResponses( ([
                "default" : (: cmdParse :),
                ]) );
    SetRequestResponses( ([
                "default" : (: cmdParse :),
                ]) );
}

string SetBankName(string str){ return (BankName = str); }

string SetLocalCurrency(string str){ return (LocalCurrency = str); }

int SetLocalFee(int x){ return (LocalFee = x); }

int SetNonLocalFee(int x){ return (NonLocalFee = x); }

int SetOpenFee(int x){ return (OpenFee = x); }

float SetExchangeFee(float x){ return (ExchangeFee = x); }

mixed SetCurrencies(mixed var){
    if( arrayp(var) ){
        if( member_array(LocalCurrency, var) == -1 )
            var += ({ LocalCurrency });
        return (Currencies = var);
    }
    else error("Bad argument 1 to SetCurrencies()");
}

int GetLocalFee(){ return LocalFee; }

int GetNonLocalFee(){ return NonLocalFee; }

int GetExchangeFee(){ return ExchangeFee; }

int GetOpenFee(){ return OpenFee; }

mixed GetCurrencies(){ return copy(Currencies); }

string GetBankName(){ return BankName; }

string GetLocalCurrency(){ return LocalCurrency; }

mixed CanBank(object who){
    if( !who->GetAccountInfo( GetBankName() ) )
        return "You must create an account with "+GetBankName()+" first.";
    return 1;
}

int AddSurcharge(object who, string currency, int amount){
    int charge;
    string type;

    if( who->GetTown() == GetTown() ){
        type = "standard";
        charge = GetLocalFee();
    } else {
        type = "non-residential";
        charge = GetNonLocalFee();
    }
    if( !charge ) return amount;
    if( amount > charge ){
        amount -= charge;
        who->eventPrint(sprintf("The bank retains a %s "
                    "surcharge of %d %s.", type, charge, currency));
        return amount;
    }
    else {
        who->eventPrint(sprintf("You are unable to afford the "
                    "%s surcharge of %d %s.", type, charge, currency));
        return 0;
    }
    return amount;
}

int eventBalance(object who){
    string str, ret, *tmp;
    mapping mp;
    float val;
    int total;

    mp = who->GetAccountInfo( GetBankName() );
    if( !mp ) return 0;
    environment()->eventPrint( who->GetName() + " requests "
            "current balance.", ({ who }) );
    ret = "Your account with "+GetBankName()+" was "
        "opened "+ctime(mp["open"])+".\n";
    ret += "This location's default currency is "+GetLocalCurrency()+".\n\n";
    tmp = filter(keys(mp), (: member_array($1, Currencies) > -1 :));
    total = 0;
    if( !sizeof(tmp) )
        ret += "You have made no deposits.\n";
    else foreach(str in tmp){
        if( mp[str] < 1 ) continue;
        ret += sprintf("%-20s : %d\n", str+"", mp[str]);
        if( str == GetLocalCurrency() ) total += mp[str];
        else if( mp[str] ){
            if( currency_rate(str) )
                val = query_value(mp[str],str,GetLocalCurrency()); 
            else val = 0;
            if(val) total += val;
        }
    }
    ret += sprintf("%-20s ----------\n%-22s %d",
            "", "Total in "+GetLocalCurrency(), total);
    if( mp["last time"] )
        ret += sprintf("\n\nYour last transaction: %s at %s.",
                mp["last trans"], ctime(mp["last time"]) );
    who->eventPage(explode(ret, "\n"), "info");
    return 1;
}

int eventDeposit(object who, string currency, int amount){
    int i;
    if( amount < 1 ){
        eventForce("speak " + who->GetName() + ", "
                "That transaction is unacceptable.");
        return 1;
    }
    if( !currency ) currency = GetLocalCurrency();
    if( !(i = who->GetCurrency(currency)) ){
        eventForce("speak You are not carrying any "+currency+".");
        return 1;
    }
    if( amount > i ){
        eventForce("speak You are not carrying that much "+ currency +".");
        return 1;
    }
    who->eventPrint(sprintf("You deposit %d %s into your "
                "account.", amount, currency));
    environment()->eventPrint(who->GetName() + " makes "
            "a deposit.", ({ who }) );
    who->AddCurrency(currency, -amount);
    who->AddBank(GetBankName(), currency, amount);
    return 1;
}

int eventWithdraw(object who, string currency, int amount){
    int i, x, charge;

    x = amount;
    if( amount < 1 ){
        eventForce("speak Withdrawing a negative amount is not a prudent "
                "idea, "+ who->GetName() +".");
        return 1;
    }
    if( !currency ) currency = GetLocalCurrency();
    if( !(i = who->GetBank(GetBankName(), currency)) ){
        eventForce("speak You have no "+currency+" in your account.");
        return 1;
    }
    if( amount > i ){
        eventForce("speak You do not have that much "+currency+" in your "
                "account.");
        return 1;
    }
    if( !(amount = AddSurcharge(who, currency, amount)) ) return 1;
    charge = x - amount;
    if( who->AddCurrency(currency, x) < 0 ){
        eventForce("speak You are unable to carry that "
                "much "+currency+"!");
        who->eventPrint("The bank credits your account with the fee.");
        return 1;
    }
    who->AddCurrency(currency, -charge);
    who->AddBank(GetBankName(), currency, -x);
    who->eventPrint(sprintf("You withdraw %d %s from your account.",
                amount, currency));
    environment()->eventPrint(sprintf("%s withdraws some %s.",
                who->GetName(), currency), ({ who }));
    return 1;
}

int eventExchange(object who, int amount, string str1, string str2){
    int i, x, charge;
    float val;

    if( amount < 0 ){
        amount = who->GetCurrency(str1);
    }
    if( amount < 1 ){
        eventForce("speak " + who->GetName() + ", that "
                "transaction cannot be completed.");
        return 1;
    }
    if( amount > who->GetCurrency(str1) ){
        eventForce("speak You do not have enough of that currency to exchange.");
        return 1;
    }
    if( str1 == str2 ){
        eventForce("speak That would be pointless.");
        return 1;
    }
    if( member_array(str1, GetCurrencies()) == -1 ){
        eventForce("speak You cannot exchange that currency here.");
        return 1;
    }
    if( member_array(str2, GetCurrencies()) == -1 ){
        eventForce("speak You cannot exchange that currency here.");
        return 1;
    }
    val = query_base_value(str1,amount);
    x = to_int( (amount * 0.01) + amount );
    i = val / currency_rate(str2);
    if( GetExchangeFee() )
        charge = to_int(i / (100 / GetExchangeFee()));
    else charge = 0;
    if( charge ) i -= charge;
    if( i < 1 ){
        eventForce("speak That isn't even worth one " + str2 + "!");
        return 1;
    }
    if( who->AddCurrency(str2, i) < 0 ){
        eventForce("speak You cannot carry that much "+str2+", " +
                who->GetName()+".");
        return 1;
    }
    who->AddCurrency(str1, -amount);
    who->eventPrint(sprintf("You exchange %d %s for %d %s.",
                amount, str1, i, str2));
    if( charge )
        who->eventPrint(sprintf("You were charged a fee of %d %s.",      
                    charge, str2) );
    environment()->eventPrint(who->GetName()+" exchanges some "+
            str1+" for "+str2+".", who);
    return 1;
}

int eventOpenAccount(object who){
    if( who->GetAccountInfo( GetBankName() ) ){
        eventForce("speak You already have an account with "+GetBankName()+", " +
                who->GetName()+"!");
        return 1;
    }
    if( who->GetCurrency( GetLocalCurrency() ) < OpenFee ){
        eventForce(" speak "+ sprintf("You must have at least %d %s to open "
                    "an account at this branch of %s, %s.", GetOpenFee(),
                    GetLocalCurrency(), GetBankName(), who->GetName()));
        return 1;
    }
    if( OpenFee < 1 ) return 0;
    who->AddCurrency(GetLocalCurrency(), -OpenFee);
    who->AddBank(GetBankName(), GetLocalCurrency(), OpenFee);
    who->eventPrint(sprintf("You deposit %d %s and open an account "
                "with %s.", OpenFee, GetLocalCurrency(), GetBankName()) );
    eventBalance(who);
    environment()->eventPrint(who->GetName() + " opens an "
            "account.", who);
    eventForce("smile "+who->GetKeyName());
    return 1;
}

int cmdParse(object who, string cmd, string str, mixed args...){
    int x;
    string s1, s2, honorific;
    mixed err;

    if( cmd != "account" && stringp(err = CanBank(who)) ){
        who->eventPrint(err);
        return 1;
    }
    switch(cmd){
        case "balance" :
            return eventBalance(who);
            break;
        case "deposit" : 
            if( !str ){
                eventForce("speak How much of what?");
                return 1;
            }
            if(str == "all"){
                if(!sizeof(this_player()->GetCurrencies())){
                    who->eventPrint("You have no money to deposit.");
                    return 1;
                }
                foreach(string dinero in this_player()->GetCurrencies()){
                    if(this_player()->GetCurrency(dinero) > 0)
                        eventDeposit(who, dinero, this_player()->GetCurrency(dinero));
                }
                return 1;
            }
            sscanf(str, "%d %s", x, s1);
            if( member_array(s1, Currencies) < 0 ){
                who->eventPrint("That's not a valid currency.");
                return 1;
            }
            return eventDeposit(who, s1, x);
            break;
        case "withdraw" :
            if( !str ){
                eventForce("speak How much of what?");
                return 1;
            }
            if(str == "all"){
                if(this_player()->GetGender() == "female") honorific = "ma'am";
                else honorific = "sir";
                eventForce("speak I'm sorry, "+honorific+", but withdrawals must be specified explicitly in terms of currency type and amount.");
                return 1;
            }
            sscanf(str, "%d %s", x, s1);
            if( member_array(s1, Currencies) < 0 ){
                if(!s1) who->eventPrint("Please specify a currency.");
                else who->eventPrint(s1 + " is not supported here!");
                return 1;
            }
            return eventWithdraw(who, s1, x);
            break;
        case "exchange" :
            if( !str ){
                eventForce("speak Exchange what for what?");
                return 1;
            }
            if(sscanf(str, "%d %s for %s", x, s1, s2) < 3){
                x = -1;
                sscanf(str, "%s for %s", s1, s2);
            }        
            if(s1 == "all"){
                int ret = 0;
                foreach(string curr in (who->GetCurrencies() - ({ s2 }))){
                    if(who->GetCurrency(curr) < 1) continue;
                    ret += eventExchange(who, -1, curr, s2);
                }
                return ret;
            }
            else return eventExchange(who, x, s1, s2);
            break;
        case "account" :
            return eventOpenAccount(who);
            break;
        default:
            eventForce("speak " + who->GetName() + ", I do not "
                    "understand what you want.");
            eventForce("whisper to " + who->GetKeyName() + " Try "
                    "\"help banking\" if you are confused.");

    }
    return 1;
}
