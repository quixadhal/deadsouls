#ifndef __CURRENCY_H__
#define __CURRENCY_H__

static void create();

int CanCarry(int amount);

int AddCarriedMass(int amount);
int AddCurrency(string type, int amount);
int GetCurrency(string type);
varargs int GetCurrencyMass(string type);
int AddBank(string bank, string type, int amount);
int GetBank(string bank, string type);
mapping GetAccountInfo(string bank);
int GetNetWorth();
string *GetCurrencies();

#endif /* __CURRENCY_H__ */
