#ifndef l_corpse_h
#define l_corpse_h

static void create();

int eventDecay();
int eventDestruct();

int SetDecayLife(int x);
int GetDecayLife();
void SetCorpse(object who);
string GetOwner();
string GetRace();

#endif /* l_corpse_h */
