#ifndef l_chapel_h
#define l_chapel_h

static void create();

mixed CanMarry(object who, object spouse1, object spouse2);
mixed CanSacrifice(object who, object what, string deus);

mixed eventMarry(object who, object spouse1, object spouse2);
mixed eventSacrifice(object who, object what, string deus);

int SetAllowSacrifice(int x);
int GetAllowSacrifice();
string *SetClasses(string *rc);
string *GetClasses();
string *SetDeities(string *deities);
string *GetDeities();
string *SetReligion(string adj, string noun);
varargs string GetReligion(int flag);
int SetSacrificeType(int x);
int AddSacrificeType(int x);
int GetSacrificeType();

#endif /* l_chapel_h */
