#ifndef l_drop_h
#define l_drop_h

static void create();

mixed can_drop_obj(string verb);
mixed can_drop_wrd_wrd(string amt, string curr);

mixed do_drop_obj(object ob);
mixed do_drop_obs(object *obs);
mixed do_drop_wrd_wrd(string amt, string curr);

#endif /* l_drop_h */
