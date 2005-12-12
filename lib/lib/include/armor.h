#ifndef l_armor_h
#define l_armor_h

static void create();
mixed direct_remove_obj();
mixed direct_wear_obj();
mixed direct_wear_obj_on_str(mixed fix...);

mixed CanWear(object who, string *limbs);
mixed CanWield(object who, string *limbs);

void eventDeteriorate(int type);
void eventEquipAgain(string *limbs);
int eventReceiveDamage(object agent, int type, int strength, int internal, mixed limbs);
mixed eventUnequip(object who);
mixed eventWear(object who, string *limbs);

string GetWornDescription();
string GetItemCondition();
int SetArmorType(int x);
int SetProtection(int type, int amount);
int GetProtection(int type);
int GetMaxProtection(int type);
string *SetRestrictLimbs(string *limbs);
string *GetRestrictLimbs();
int SetFingers(int x);
int GetFingers();
mixed SetWear(mixed val);
mixed GetWear();
string GetShort();

#endif /* l_armor_h */


