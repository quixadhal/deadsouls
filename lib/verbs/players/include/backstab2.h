#ifndef l_backstab_h
#define l_backstab_h

static void create();
mixed can_backstab_liv(object ob);
mixed do_backstab_liv(object ob);
string GetHelp(string str);
int eventBackstab(object backstabber, object target, int suprise);
int eventStab(object backstabber, object target, object weapon);
int eventPrintDamage(object backstabber, object target, object weapon, int percentDamage);

#endif /* l_backstab_h */


