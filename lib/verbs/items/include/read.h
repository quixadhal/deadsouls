#ifndef l_read_h
#define l_read_h

static void create();

mixed can_read_obj(string verb, string id);
mixed can_read_str_word_obj(string str, string verb);

varargs mixed do_read_obj(mixed *args...);
mixed do_read_str_from_obj(string str, object ob);
mixed do_read_str_on_obj(string str, object ob);

#endif /* l_read_h */
