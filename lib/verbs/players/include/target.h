#ifndef l_target_h
#define l_target_h

static void create();

mixed can_target_liv(object target);

mixed do_target_liv(object target);
varargs mixed do_target_lvs(mixed *targets, int exclusive);

#endif /* l_target_h */
