#ifndef l_round_h
#define l_round_h

static void create();

int SetCaliber(int x);
int SetMillimeter(int x);
int SetPistolType(string str);
int SetAmmoType(string str);
int SetRifleType(string str);
string GetType();
int GetCaliber();
int GetMillimeter();
string GetAmmoType();
string GetRifleType();

#endif /* l_round_h */
