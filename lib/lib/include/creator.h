#ifndef __CREATOR_H__
#define __CREATOR_H__

static void create();
static void net_dead();
void restart_heart();
mixed direct_can_give_liv_obj();
mixed indirect_give_obj_to_liv(object item);

int cmdListen(string str);
int cmdSearch(string str);
int cmdSmell(string str);
int cmdTouch(string str);

int CanCarry(int amount);

int eventForce(string cmd);
void eventDescribeEnvironment(int verbose);
varargs int eventMoveLiving(mixed dest, string omsg, string msg);
varargs mixed eventShow(object who, string str, string on_id);

int Setup();

int GetCreatorAge();
string SetGender(string gender);
string GetGender();
varargs string GetLong(string blah);
int GetCreatorBirth();
string GetNativeLanguage();
int GetLanguageLevel(string whocares);

#endif /* __CREATOR_H__ */
