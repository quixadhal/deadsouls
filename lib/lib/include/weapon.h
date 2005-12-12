#ifndef l_weapon_h
#define l_weapon_h

static void create();
static void init();
mixed direct_remove_obj();
mixed direct_unwield_obj();
mixed direct_wear_obj();
mixed direct_wield_obj();
mixed direct_wield_obj_word_str(mixed fix...);

mixed CanWield(object who, string *limbs);

void eventDeteriorate(int type);
void eventEquipAgain(string *limbs);
int eventStrike(object ob);
mixed eventWield(object who, string *limbs);
mixed eventUnequip(object who);

int SetArmorType(int x);
int GetArmorType();
int SetClass(int x);
mixed GetProperty(string foo);
int GetClass();
int SetMaxClass(int x);
int GetMaxClass();
int AddClassBonus(int amount, int time);
int RemoveClassBonus(int amount);
int GetCurrentBonus();
int SetDamageType(int x);
int GetDamageType();
string GetEquippedShort();
int SetHands(int x);
int GetHands();
mixed GetLimbList(string str);
string SetWeaponType(string str);
string GetWeaponType();
mixed SetWield(mixed val);
mixed GetWield();
static string *SetWorn(string *limbs);
string *GetWorn();

/* pure virtual */ string GetKeyName();
/* pure virtual */ int AddPoison(int x);
/* pure virtual */ int GetPoison();
/* pure virtual */ string GetShort();

#endif /* l_weapon_h */
