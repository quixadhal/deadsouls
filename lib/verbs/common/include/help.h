#ifndef l_help_h
#define l_help_h

static void create();
mixed can_help_str(string str);
mixed do_help_str(string str);
int CanAccess(object who, string index);
varargs void HelpMenu(string index);
static private void LoadIndices();
string GetHelp(string str);
string GetHelpByIndex(string index, string topic);
varargs string *GetIndices(string topic);
string GetTopic(string index, string topic);

#endif /* l_help_h */
