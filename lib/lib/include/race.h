#ifndef l_race_h
#define l_race_h

static void create();

mixed CanDrink(object ob);
mixed CanEat(object ob);

varargs int eventDie(object agent);
mixed eventDrink(object ob);
mixed eventEat(object ob);

varargs string SetRace(string race, mixed extra);
string GetRace();
string SetGender(string gender);
string GetGender();
varargs void SetStat(string stat, int level, int classes);
varargs int GetMaxHealthPoints(string limb);
int GetMaxMagicPoints();
float GetMaxStaminaPoints();
void NewBody(string race);
string SetTown(string str);
string GetTown();
int GetLuck();
int GetMobility();
int GetCarriedMass();
int GetMaxCarry();
int GetHeartRate();
int GetHealRate();
int GetStatLevel(string stat);
int GetAlcohol();

#endif /* l_race_h */


