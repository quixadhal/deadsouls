#ifndef l_smell_h
#define l_smell_h

static void create();
mixed direct_smell_obj();
mixed direct_smell_str_word_obj(string str);

varargs mixed eventSmell(object who, string id);

varargs mixed SetSmell(mixed arg1, mixed desc);
mapping RemoveSmell(string item);
string GetSmell(string str);
string *GetSmells();

#endif /* l_smell_h */
