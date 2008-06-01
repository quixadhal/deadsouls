#ifndef l_pistol_h
#define l_pistol_h

static void create();

int eventShoot(object who);

int InitRevolver(mixed array);
int SetAmmoType(string str);
int SetFirearmType(string str);
int SetMaxAmmo(int x);
int SetCaliber(float f);
int SetMillimeter(int x);
string GetAmmoType();
string GetFirearmType();
int GetMaxAmmo();
int GetCaliber();
int GetMillimeter();
int CalculateAmmoSize();
int do_shoot(string str);
int missed_shot();
int AddRound(int x);
int SubtractRound(int x);
int ShowRounds();
int LoadRevolver(string str);
int LoadAuto(string str);
int doRevolverUnload(string foo, string bar);
int doAutoUnload(string str);
int UnloadAll();
int SetAmmoFile(string str);

#endif /* l_pistol_h */
