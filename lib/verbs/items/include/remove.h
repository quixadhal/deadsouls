#ifndef l_remove_h
#define l_remove_h

static void create();

mixed can_remove_obj(string verb);
mixed can_remove_obj_from_obj();
mixed can_remove_obj_out_of_obj();

mixed do_remove_obj(object ob);
mixed do_remove_obs(mixed *res);
mixed do_remove_obj_from_obj(object targ, object container);
mixed do_remove_obj_out_of_obj(object targ, object container);
mixed do_remove_obs_from_obj(mixed *res, object container);
mixed do_remove_obs_out_of_obj(mixed *res, object container);

#endif /* l_remove_h */
