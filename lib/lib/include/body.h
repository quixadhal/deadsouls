#ifndef l_body_h
#define l_body_h
 
static void create();
static void heart_beat();
void restart_heart();

int CanFly();

void eventCheckHealing();
void eventCompleteHeal(int x);
mixed eventFall();
varargs int eventHealDamage(int x, int internal, mixed limbs);
varargs int eventReceiveDamage(object agent, int type, int x, int internal, mixed limbs);
int eventCheckProtection(object agent, int type, int damage);
 
mixed eventReceiveThrow(object who, object what);
varargs int eventDie(object agent);
int eventRemoveItem(object ob);
int eventWear(object ob, mixed limbs);
 
void NewBody(string race);
mixed CanWear(object armour, string *limbs);
varargs int AddLimb(string limb, string parent, int classes, int *armours);
int RestoreLimb(string str);
int RemoveLimb(string limb, object agent);
mapping GetLimb(string limb);
string GetRandomLimb(string targ);
string GetTorso();
string *GetLimbs();
int GetLimbClass(string limb);
string GetLimbParent(string limb);
string *GetLimbChildren(string limb);
mapping GetMissingLimb(string limb);
string *GetMissingLimbs();
string GetLong(string nom);
string *GetWieldingLimbs();
varargs int AddFingers(string limb, int x);
int GetFingers(string limb);
varargs object *GetWorn(string limb);
varargs mixed GetWielded(string limb);
varargs static int AddHealthPoints(int x, string limb, object agent);
varargs int GetHealthPoints(string limb);
varargs int GetMaxHealthPoints(string limb);
int AddMagicPoints(int x);
int GetMagicPoints();
int GetMaxMagicPoints();
float AddStaminaPoints(mixed x);
int GetStaminaPoints();
float GetMaxStaminaPoints();
int AddMagicProtection(class MagicProtection cl);
class MagicProtection *GetMagicProtection();
int RemoveMagicProtection(int i);
int SetUndead(int x);
int GetUndead();
string SetUndeadType(string str);
string GetUndeadType();
int SetSleeping(int x);
int GetSleeping();
int AddAlcohol(int x);
int GetAlcohol();
int AddCaffeine(int x);
int GetCaffeine();
int AddFood(int x);
int GetFood();
int AddDrink(int x);
int GetDrink();
int AddPoison(int x);
int GetPoison();
int GetDying();
string GetResistance(int type);
string GetRace();
string GetName();
string GetCapName();
int GetHeartRate();
int GetHealRate();
/* pure virtual */ string GetShort();
/* pure virtual */ varargs mixed eventPrint(mixed msg, mixed cl, mixed three);
/* pure virtual */ varargs void SetParalyzed(int x, function f);
/* pure virtual */ varargs mixed eventMoveLiving(mixed dest, string omsg,
						 string imsg);

#endif /* l_body_h */
