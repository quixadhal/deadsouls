#ifndef l_climb_h
#define l_climb_h

static void create();

mixed can_climb_obj(string verb);
mixed can_climb_word_obj(string verb, string word);
mixed can_climb_out_of_obj(string verb);

mixed do_climb_obj(object ob, string id);
mixed do_climb_word_obj(string word, object ob, string id);
mixed do_climb_out_of_obj(object ob, string id);

#endif /* l_climb_h */
