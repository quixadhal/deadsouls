#ifndef __LIMB_H__
#define __LIMB_H__

static void create();

int eventDecay();

int SetDecayLife(int x);
int GetDecayLife();
void SetLimb(string limb, string owner, string race);
string GetLimb();
string GetOwner();
string GetRace();

#endif /* __LIMB_H__ */
