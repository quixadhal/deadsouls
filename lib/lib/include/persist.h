#ifndef l_persist_h
#define l_persist_h

static int eventConvertObject(mixed val, int recurse);
int eventLoadObject(mixed val, int recurse);

static mixed *AddSave(mixed *vars);
mixed *GetSave();
static int SetSaveRecurse(int flag);
string GetSaveString();

/* pure virtual */int eventMove(mixed dest);

#endif /* l_persist_h */
