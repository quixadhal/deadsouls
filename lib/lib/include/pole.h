#ifndef l_pole_h
#define l_pole_h

static void create();
mixed direct_bait_obj_with_obj();
mixed direct_fish_with_obj();

mixed CanCast(object who);

int eventBait(object who, object bait);
int eventBreak();
mixed eventCatch(object who, string fish);
int eventFish(object who);

int SetBait(int x);
int AddBait(int x);
int GetBait();
int SetChance(int x);
mixed GetProperty(string foo);
int GetChance();
int SetStrength(int x);
int GetStrength();

/* virtual functions */

mixed AddSave(mixed *vars);
string GetShort();
int SetBroken(int x);
int GetBroken();


#endif /* l_pole_h */
