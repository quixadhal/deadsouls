#ifndef l_scroll_h
#define l_scroll_h

static void create();
mixed direct_use_obj_to_str(string str);

mixed CanUse(object who, string str);

mixed eventUse(object who, string str);

string *SetCommands(string *cmds);
string *AddCommand(mixed arg);
string *RemoveCommand(mixed arg);
string *GetCommands();

#endif /* l_scroll_h */
