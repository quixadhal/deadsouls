#ifndef l_give_h
#define l_give_h

static void create();

mixed can_give_liv_obj();
mixed can_give_obj_to_liv();
mixed can_give_liv_wrd_wrd(object targ, string num, string curr);
mixed can_give_wrd_wrd_to_liv(string num, string curr, object targ);

mixed do_give_liv_obj(object target, object what);
mixed do_give_obj_to_liv(object what, object target);
mixed do_give_liv_wrd_wrd(object target, string num, string curr);
mixed do_give_wrd_wrd_to_liv(string num, string curr, object target);
mixed do_give_liv_obs(object target, mixed *items);
mixed do_give_obs_to_liv(mixed *items, object target);

#endif /* l_give_h */
