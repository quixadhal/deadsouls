#ifndef l_party_h
#define l_party_h

static void create();
int livings_are_remote();
mixed can_party_wrd(string cmd);
mixed can_party_wrd_wrd(string cmd, string party);
mixed can_party_wrd_liv(string cmd);
mixed do_party_wrd(string cmd);
mixed do_party_wrd_wrd(string cmd, string party);
mixed do_party_wrd_liv(string cmd, object targ);
string GetHelp(string str);

#endif /* l_party_h */
