#ifndef l_put_h
#define l_put_h

static void create();

mixed can_put_obj_word_obj(object target, string wrd, object storage);

mixed do_put_obj_word_obj(object what, string wrd, object storage);
mixed do_put_obs_word_obj(mixed *res, string wrd, object storage);

mixed eventCheckLight(object who);

#endif /* l_put_h */
