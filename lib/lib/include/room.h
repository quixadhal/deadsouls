#ifndef l_room_h
#define l_room_h

static void create();
varargs void reset(int count);
static void init();
int id(string str);

varargs mixed eventHearTalk(object who, object target, int cls, string verb, string msg, string lang);
int eventMove(mixed dest);
varargs int eventPrint(string msg, mixed arg2, mixed arg3);
varargs int eventShow(object who, string str);
int CanAttack(object attacker, object who);

static void LoadInventory();

int GetAmbientLight();
void SetShort(string str);
void SetLong(string str);
string GetShort();
varargs string GetLong(string str);
object *AddExtraLong(object ob);
string GetExtraLong();
int GetResetNumber();
string *GetId();
string SetDayLong(string str);
string GetDayLong();
string SetNightLong(string str);
string GetNightLong();
string SetClimate(string str);
string GetClimate();
float SetGravity(float h);
float GetGravity();
mapping SetInventory(mapping mp);
mapping GetInventory();
int GetDayLight();
static int SetDayLight(int x);
int GetNightLight();
static int SetNightLight(int x);
int SetNoReplace(int x);
int GetNoReplace();
int GetShade();
static int SetShade(int x);

#endif /* l_room_h */
