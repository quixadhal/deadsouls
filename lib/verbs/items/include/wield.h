#ifndef l_wield_h
#define l_wield_h

static void create();

mixed can_wield_obj();
mixed can_wield_obj_word_str(string str);

mixed do_wield_obj(object ob);
mixed do_wield_obj_word_str(object ob, string word, string str);

#endif /* l_wield_h */
