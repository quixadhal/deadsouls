#ifndef l_echo_h
#define l_echo_h

static void create();
mixed can_echo();
mixed can_echo_to_liv_str(object who, string what);
mixed can_echo_str(string str);
mixed do_echo();
mixed do_echo_to_liv_str(object who, string what);
mixed do_echo_str(string str);
void eventEcho(object *targs, string str);
string GetHelp(string str);

#endif /* l_echo_h */
