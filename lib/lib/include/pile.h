#ifndef l_pile_h
#define l_pile_h

static void create();
static void init();
mixed direct_get_wrd_wrd_from_obj(string num, string curr);
mixed direct_get_wrd_wrd_out_of_obj(string num, string curr);

mixed eventGetMoney(object who, int amount, string curr);

string *GetId();
string GetShort();
varargs string GetLong(string str);
void SetPile(string str, int amt);
string GetPileType();
int GetPileAmount();
int GetMass();

#endif /* l_pile_h */
