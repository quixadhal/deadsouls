

#ifndef l_clan_h
#define l_clan_h


class ClanClass {
    string leader;
    string name;
    string objectName;
    string skill;
}

static void create();
static void init();

mixed CanJoin(object ob);
string GetAffectLong(object ob);
string SetLeader(string str);
string GetLeader();
string SetClanName(string str);
string GetClanName();
string SetClanObject(string str);
string GetClanObject();
string SetClanSkill(string str);
string GetClanSkill();

int eventBring(string str);
int eventInitiate(string str);
void eventJoin(object ob);
int eventRetire(string str);
void eventUnjoin(object ob);
void eventWelcome(object ob);


#endif /* l_clan_h */


