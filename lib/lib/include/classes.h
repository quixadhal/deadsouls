#ifndef l_classes_h
#define l_classes_h

static void create();

int eventMoralAct(int degree);

int AddSkillPoints(string skill, int x);
static string SetSkillModifier(string skill, string stat);
string GetSkillModifier(string skill);
string SetClass(string classes);
string GetClass();
string SetGuild(string guild);
string GetGuild();
int GetBaseStatLevel(string stat);
int SetMorality(int x);
int GetMorality();
string GetMoralityDescription();
string *SetReligion(string adj, string noun);
varargs string GetReligion(int flag);

#endif /* l_classes_h */
