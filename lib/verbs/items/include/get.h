#ifndef l_get_h
#define l_get_h

static void create();

mixed can_get_obj(string verb);
mixed can_get_obj_from_obj(string verb);
mixed can_get_obj_out_of_obj(string verb);
mixed can_get_wrd_wrd_from_obj(string num, string curr);
mixed can_get_wrd_wrd_out_of_obj(string num, string curr);

mixed do_get_obj(object ob);
mixed do_get_obj_from_obj(object target, object storage);
mixed do_get_obj_out_of_obj(object target, object storage);
mixed do_get_wrd_wrd_from_obj(string num, string curr, object pile);
mixed do_get_wrd_wrd_out_of_obj(string num, string curr, object pile);
mixed do_get_obs(object *obs);
mixed do_get_obs_from_obj(object *obs, object ob);
mixed do_get_obs_out_of_obj(object *obs, object ob);

mixed eventCheckLight(object who);

#endif /* l_get_h */
