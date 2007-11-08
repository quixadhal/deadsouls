#ifndef l_interactive_h
#define l_interactive_h

static void create();
int inventory_accessible();
int inventory_visible();

mixed CanDivorce();
mixed CanGet();
mixed CanMarry();

int Setup();
static void net_dead();
void eventReconnect();
void eventDescribeEnvironment(int brief);
int eventDestruct();
mixed eventDivorce();
mixed eventMarry(object who, object to_whom);
int eventMove(mixed dest);
void cmdParseRefresh();
int cmdQuit();
int GetAge();
int GetBirth();
string query_cwd();
void SetEmail(string str);
string GetEmail();
void SetId(string *bogus);
string *GetId();
int id(string str);
int GetLoginTime();
string SetKeyName(string str);
string GetKeyName();
void SetNews(string type, int sz);
int GetNews(string type);
void SetPassword(string str);
string GetPassword();
void SetRank(string str);
string GetRank();
void SetRealName(string str);
string GetRealName();
string GetShort();
string GetLong();
int SetBriefMode(int x);
int GetBriefMode();
int SetWhereBlock();
int GetWhereBlock();
string get_path();
void SetLastError(mapping m);
mapping GetLastError();
void SetCapName(string str);
void move_or_destruct();
string SetShort(string str);
string SetLong(string str);
string GetName();
varargs int GetInvis(object ob);
mixed *GetCommands();
string GetSpouse();
int GetRadiantLight(int ambient);

void eventLoad(mixed *value, int recurse);

/* pure virtual */ varargs mixed GetEffectiveVision(mixed location, int raw_score);

#endif /* l_interactive_h */
