#ifndef l_lock_h
#define l_lock_h

static void create();

mixed can_lock_obj_with_obj(string verb);

varargs mixed do_lock_obj_with_obj(object ob, object key, mixed *args...);

#endif /* l_lock_h */
