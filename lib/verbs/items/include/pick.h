#ifndef l_pick_h
#define l_pick_h

static void create();

mixed can_pick_str_on_obj(string str);
mixed can_pick_str_on_obj_with_obj(string str);

varargs mixed do_pick_str_on_obj(string str, object ob, mixed *args...);
varargs mixed do_pick_str_on_obj_with_obj(string str, object ob, object tool,
				 mixed *args...);

#endif /* l_pick_h */
