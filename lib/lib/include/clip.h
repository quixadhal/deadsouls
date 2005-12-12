#ifndef l_clip_h
#define l_clip_h

static void create();

int PlusAmmo(int i);
int MinusAmmo(int i);
int SetMaxAmmo(int x);
int SetCaliber(int x);
int SetMillimeter(int x);
int SetPistolType(string str);
int SetAmmoType(string str);
string GetType();
int GetCaliber();
int GetMillimeter();
string GetAmmoType();

#endif /* l_clip_h */
