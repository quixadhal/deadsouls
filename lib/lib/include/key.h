#ifndef l_key_h
#define l_key_h

static void create();
mixed indirect_lock_obj_with_obj(object target, object key, string id);
mixed indirect_unlock_obj_with_obj(object target, object key, string id);

mixed CanLockLock(object who, string id, object what);
mixed CanUnlockLock(object who, string id, object what);

mixed eventLockLock(object who, string id, object what);
mixed eventUnlockLock(object who, string id, object what);

int SetDisableChance(int x);
int GetDisableChance();
int SetDisabled(int x);
int GetDisabled();

/* pure vitual */ static mixed *AddSave(mixed *vars);
/* pure virtual */ string GetShort();

#endif /* l_key_h */
