#ifndef l_jump_h
#define l_jump_h

static void create();

mixed can_jump_word_obj();
mixed can_jump_word_word_obj();

varargs mixed do_jump_in_obj(object ob, mixed *args...);
varargs mixed do_jump_into_obj(object ob, mixed *args...);
varargs mixed do_jump_off_obj(object ob, mixed *args...);
varargs mixed do_jump_off_of_obj(object ob, mixed *args...);
varargs mixed do_jump_from_obj(object ob, mixed *args...);
varargs mixed do_jump_through_obj(object ob, mixed *args...);
varargs mixed do_jump_across_obj(object ob, mixed *args...);
varargs mixed do_jump_over_obj(object ob, mixed *args...);
varargs mixed do_jump_onto_obj(object ob, mixed *args...);
varargs mixed do_jump_on_obj(object ob, mixed *args...);

#endif /* l_jump_h */

