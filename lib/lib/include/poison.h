#ifndef l_poison_h
#define l_poison_h

static void create();
mixed indirect_poison_obj_with_obj();

mixed eventSpreadPoison(object who, object target);

int SetPoisonStrength(int x);
int GetPoisonStrength();
int SetPoisonUses(int x);
int GetPoisonUses();

#endif /* l_poison_h */
