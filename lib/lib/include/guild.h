

#ifndef l_guild_h
#define l_guild_h


class GuildClass {
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
string SetGuildName(string str);
string GetGuildName();
string SetGuildObject(string str);
string GetGuildObject();
string SetGuildSkill(string str);
string GetGuildSkill();

int eventBring(string str);
int eventInitiate(string str);
void eventJoin(object ob);
int eventRetire(string str);
void eventUnjoin(object ob);
void eventWelcome(object ob);


#endif /* l_guild_h */


