#ifndef l_item_h
#define l_item_h

static void create();
static void init();
varargs void reset(int count);
mixed direct_bury_obj_with_obj();
mixed indirect_give_liv_obj(object target);
mixed direct_give_obj_to_liv();
mixed direct_look_at_obj(string id);
mixed direct_look_at_obj_word_obj(string my_id, string id);
mixed direct_poison_obj_with_obj();

mixed CanSteal(object who);
mixed CanEnter(object who, string what);
int CanSell();
varargs mixed CanThrow(object who, object target);
mixed CanRepair(object who);

mixed eventDetect(object who, string str, int ability);
int eventMove(mixed dest);
int eventReceiveDamage(object agent, int type, int amount, int unused, mixed limbs);
int eventBless(int amount, int time);
void eventRemoveBlessing();
varargs mixed eventShow(object who, string str, string on_id);
varargs mixed eventThrow(object who, object target);
mixed eventPoison(object who, object agent, int strength);
varargs mixed eventRepair(object who, int strength, int type);

static int Destruct();

string GetWornDescription();
string GetItemCondition();
int SetMass(int x);
int GetMass();
int GetWeight();
int SetDestroyOnSell(int x);
int GetDestroyOnSell();
int SetValue(int x);
int GetValue();
int SetVendorType(int type);
int GetVendorType();
int SetDamagePoints(int x);
int GetDamagePoints();
int SetBroken(int x);
int GetBroken();
int AddPoison(int x);
int GetPoison();
int SetRetainOnDeath(int x);
int GetRetainOnDeath();

#endif /* l_item_h */
