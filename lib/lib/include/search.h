#ifndef l_search_h
#define l_search_h

static void create();
mixed direct_search_obj();
mixed direct_search_str_word_obj(string str);

varargs mixed CanSearch(object who, string id);

varargs mixed eventSearch(object who, string id);

varargs mixed SetSearch(mixed arg1, mixed desc);
mapping RemoveSearch(string item);
mixed GetSearch(string str);
string *GetSearches();

#endif /* l_search_h */
