#ifndef __MONEY_H__
#define __MONEY_H__

static void create();
static void init();

int eventMove(mixed dest);

int AddCurrency(string curr, int x);
int GetCurrency(string curr);
string GetMoneyLong(string str);

#endif /* __MONEY_H__ */
