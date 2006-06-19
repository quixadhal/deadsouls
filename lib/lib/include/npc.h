#ifndef l_npc_h
#define l_npc_h

static void create();
static void init();
static void heart_beat();
void receive_message(string cl, string msg);
void catch_tell(string msg);
void restart_heart();
int is_living();
int inventory_accessible();
int inventory_visible();

int cmdListen(string str);
int cmdSearch(string str);
int cmdSmell(string str);
int cmdTouch(string str);
static int cmdAll(string arg);

varargs int eventDie(mixed agent);
int eventMove(mixed dest);
int eventCompleteMove(mixed dest);
int eventFollow(object dest, int followChance);
varargs int eventMoveLiving(mixed dest, string omsg, string imsg);
int eventReceiveObject();
int eventReleaseObject();
int eventDestruct();
void eventEnemyDied(object ob);
varargs int eventShow(object who, string str);

int CanCarry(int amount);
int CanReceive(object ob);

static int ContinueHeart();

mapping SetInventory(mapping mp);
varargs string SetRace(string race, mixed extra);
string SetClass(string cls);
int SetLevel(int x);
int GetLevel();
int SetHealthPoints(int x);
int SetMaxHealthPoints(int x);
int SetMagicPoints(int x);
float SetStaminaPoints(mixed x);
float SetMaxStaminaPoints(float x);
varargs void SetCurrency(mixed val, int amount);
int GetMaxCarry();
mixed SetEncounter(mixed val);
string *AddEncounter(string nom);
string *RemoveEncounter(string nom);
mixed GetEncounter();
mixed SetDie(mixed val);
mixed GetDie();
string SetKeyName(string nom);
string GetName();
string GetCapName();
string GetShort();
varargs string GetLong(string str);
int GetCarriedMass();
void SetAction(int chance, mixed val);
mixed GetAction();
void SetCombatAction(int chance, mixed val);
mixed GetCombatAction();
int AddCarriedMass(int x);
mixed *GetCommands();
int SetUnique(int x);
int GetUnique();
string GetCommandFail();
string *GetEnemyNames();
int GetRadiantLight(int ambient);

#endif l_npc_h
