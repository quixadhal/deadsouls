#ifndef l_listen_h
#define l_listen_h

static void create();
varargs mixed SetListen(mixed arg1, mixed desc);
mapping RemoveListen(string item);
string GetListen(string str);
string *GetListens();

#endif /* l_listen_h */
