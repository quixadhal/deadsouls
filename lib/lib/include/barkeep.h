#ifndef l_barkeep_h
#define l_barkeep_h

static void create();
static void init();
int cmdBuy(object who, string cmd, string args);
int cmdList(object who, string cmd, string args);
string SetLocalCurrency(string str);
string GetLocalCurrency();
mapping SetMenuItems(mapping mp);
mapping AddMenuItem(string item, string file);
mapping RemoveMenuItem(string item);
int GetCost(string str);
mapping GetMenuItems();

#endif /* l_barkeep_h */
