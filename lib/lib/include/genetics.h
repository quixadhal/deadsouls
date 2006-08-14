#ifndef l_genetics_h
#define l_genetics_h

static void create();

mixed eventCustomizeStat(string stat, int amount);

varargs void SetStat(string stat, int level, int classes);
varargs void AddStat(string stat, int base, int cls);
mapping GetStat(string stat);                               
int GetStatClass(string stat);
int GetBaseStatLevel(string stat);
int GetStatLevel(string stat);
int AddStatPoints(string stat, int x);
string *GetStats();
int GetMaxStatPoints(string stat, int level);
void AddStatBonus(string stat, function f);
varargs void RemoveStatBonus(string stat, object ob);
int GetStatBonus(string stat);
varargs string SetResistance(int type, string level);
string GetResistance(int type);
int GetCustomStats();
varargs mixed GetEffectiveVision(mixed location, int raw_score);
int array GetLightSensitivity();
varargs int array SetLightSensitivity(mixed array val...);
int AddVisionBonus(int x);
int GetVisionBonus();

/* pure virtual */ int GetLevel();
/* pure virtual */ int GetAlcohol();
/* pure virtual */ int GetRadiantLight(int ambient);

#endif /* l_genetics_h */
