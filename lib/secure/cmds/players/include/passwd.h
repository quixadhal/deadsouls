#ifndef l_passwd_h
#define l_passwd_h

mixed cmd(string args);
static void OldPass(string pass, object who);
static void NewPass(string pass, mixed who);
static void ConfirmPass(string str, mixed who, string newpass);
string GetHelp(string str);

#endif /* l_passwd_h */
