#ifndef l_light_h
#define l_light_h

static void create();

mixed can_light_obj();
mixed can_light_obj_with_obj();

mixed do_light_obj(object target);
mixed do_light_obj_with_obj(object target, object source);
mixed do_light_obs(mixed *targs);
mixed do_light_obs_with_obj(mixed *targs, object source);

#endif /* l_light_h */
