#ifndef l_vendor_h
#define l_vendor_h

static void create();
static void init();
mixed direct_buy_str_from_liv(string str);

mixed CanBuy(object who, object *obs);
mixed CanSell(object who, string what);

mixed eventAsk(object who, string what);
mixed eventBuy(object who, object *obs);
mixed eventSell(object who, string what);

int cmdShow(object who, string args);
int cmdBrowse(object who, string args);
int cmdAppraise(object who, string args);
int cmdPrice(object who, string args);
int GetCost(object ob, object who);
int GetValue(object ob, object who);
string SetLocalCurrency(string str);
string GetLocalCurrency();
string SetStorageRoom(string room);
string GetStorageRoom();
int SetMaxItems(int x);
int GetMaxItems();
int SetVendorType(int x);
int GetVendorType();

#endif /* l_vendor_h */
