#ifndef l_verb_h
#define l_verb_h

static void create();

varargs static string *SetRules(mixed *args...);
string *GetRules();
varargs static string *SetSynonyms(mixed *args...);
string *GetSynonyms();
static string SetVerb(string str);
string GetVerb();

#endif /* l_verb_h */
