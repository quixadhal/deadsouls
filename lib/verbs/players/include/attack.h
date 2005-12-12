#ifndef l_attack_h
#define l_attack_h

static void create();

mixed can_attack_liv(object target);

mixed do_attack_liv(object target);
varargs mixed do_attack_lvs(mixed *targets, int exclusive);

#endif /* l_attack_h */
