#ifndef l_teller_h
#define l_teller_h
 
static void create();
string SetBankName(string str);
string SetLocalCurrency(string str);
int SetLocalFee(int x);
int SetNonLocalFee(int x);
int SetOpenFee(int x);
float SetExchangeFee(float x);
mixed SetCurrencies(mixed var);
int GetLocalFee();
int GetNonLocalFee();
int GetExchangeFee();
int GetOpenFee();
mixed GetCurrencies();
string GetBankName();
string GetLocalCurrency();
int AddSurcharge(object who, string currency, int amount);
int eventBalance(object who);
int eventDeposit(object who, string currency, int amount);
int eventWithdraw(object who, string currency, int amount);
int eventExchange(object who, int amount, string str1, string str2);
int eventOpenAccount(object who);
int cmdParse(object who, string cmd, string str);
 
#endif

