#ifndef l_bank_h
#define l_bank_h
 
static void create();
 
mixed CanBank(object who, string currency);
 
string SetBank(string str);
string SetCurrency(string str);
int SetSurcharge(int amount);
int SetNonResidentSurcharge(int amount);
int SetMinimumTransaction(int amount);
float SetExchangePercentage(float f);
mixed SetCurrencies(mixed var);
string GetCurrency();
string GetBank();
int GetSurcharge(object ob);
int GetNonResidentSurcharge();
int GetMinimumTransaction();
float GetExchangePercentage();
mixed GetCurrencies();
 
int AddSurcharge(object who, string currency, int amount);
 
mixed eventDeposit(object who, string currency, int amount);
mixed eventWithdraw(object who, string currency, int amount);
mixed eventBalance(object who);
mixed eventExchange(object who, int amount, string str1, string str2);
mixed eventOpenAccount(object who);
 
#endif /* l_bank_h */
