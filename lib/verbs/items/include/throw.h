#ifndef l_throw_h
#define l_throw_h

static void create();

mixed can_throw_obj();
mixed can_throw_obj_word_obj();

mixed do_throw_obj(object ob);
mixed do_throw_obj_at_obj(object what, object where);
mixed do_throw_obj_into_obj(object what, object where);

void eventThrow(object who, object what, object where);

#endif /* l_throw_h */
