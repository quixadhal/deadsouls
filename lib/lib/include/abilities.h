#ifndef l_abilities_h
#define l_abilities_h

static void create();
varargs void SetSkill(string skill, int level, int classes);
varargs int AddSkill(string skill, int classes);
mapping GetSkill(string skill);                               
int GetSkillClass(string skill);
int GetBaseSkillLevel(string skill);
int GetSkillLevel(string skill);
int AddSkillPoints(string skill, int x);
string *GetSkills();
string *GetPrimarySkills();
void SetLevel(int x);
int ResetLevel();
int GetLevel();
int GetMaxSkillPoints(string skill, int level);
void AddSkillBonus(string skill, function f);
varargs void RemoveSkillBonus(string skill, object ob);
int GetSkillBonus(string skill);

/* pure virtual */varargs int eventPrint(string str, mixed arg2, mixed arg3);
#endif /* l_abilities_h */
