#ifndef l_player_h
#define l_player_h

class death {
    int Date;
    string Enemy;
}

static void create();
static void heart_beat();
static void net_dead();
void eventReconnect();

int cmdListen(string str);
int cmdSearch(string str);
int cmdTouch(string str);

int eventDisplayStatus();
varargs int eventDie(mixed agent);
mixed eventAsk(object who, string what);
void eventRevive();
varargs int eventShow(object who, string str);
int eventMove(mixed dest);
int eventFollow(object dest, int followChance);
varargs int eventMoveLiving(mixed dest, string omsg, string imsg);
int eventReceiveObject();
int eventReleaseObject();
void eventLoad(mixed *value, int recurse);
mixed eventUse(object used, string cmd);

int CanCarry(int amount);
int CanReceive(object ob);
mixed CanUse();

int Setup();

int AddCurrency(string type, int amount);
int AddBank(string bank, string type, int amount);
int GetMaxCarry();
string *SetTitles(string *titles);
string *AddTitle(string title);
string *RemoveTitle(string title);
string *GetTitles();
void AddQuest(string title, string desc);
mixed *GetQuests();
string SetShort(string irrelevant);
int SetUndead(int x);
string GetName();
varargs string GetLong(string str);
int AddCarriedMass(int x);
int GetCarriedMass();
string GetCapName();
int ResetLevel();
string SetClass(string str);
string SetClan(string clan);
varargs static int AddHealthPoints(int x, string limb, object agent);
int GetLanguageLevel(string lang);
int is_living();
mapping *GetDeaths();
int GetTrainingPoints();
int AddTrainingPoints(int x);
int RemoveTrainingPoints(int x);
varargs int eventTrainSkill(string skill, int points);

#endif /* l_player_h */


