#ifndef l_intermud_h
#define l_intermud_h

class list {
    int ID;
    mapping List;
}

static void create();
static void Setup();
static void eventRead(mixed *packet);
static void eventSocketClose();
static void eventConnectionFailure();
int SetDestructOnClose(int x);
int SetSocketType(int x);
string GetMudName(string mud);
mapping GetMudList();
string *GetMuds();
mapping GetChannelList();
string *GetMatch(string mud);
string GetNameserver();

#endif /* l_intermud_h */
