#ifndef l_look_h
#define l_look_h

static void create();

mixed can_look();
mixed can_look_at_str(string str, string verb);
mixed can_look_at_obj(string verb, string id);
mixed can_look_in_obj(string verb, string id);
mixed can_look_inside_obj(string verb, string id);
mixed can_look_at_obj_word_obj(string verb, string targ, string store);
mixed can_look_at_str_on_obj(string targ, string verb, string id1, string id2);

mixed do_look();
varargs mixed do_look_at_obj(object ob, mixed *args...);
mixed do_look_at_str(string str);
mixed do_look_in_obj(object ob);
mixed do_look_inside_obj(object ob);
mixed do_look_at_obj_word_obj(object target, object storage);
varargs mixed do_look_at_str_on_obj(string id, object ob, mixed args...);

#endif /* l_look_h */
