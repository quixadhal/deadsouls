#ifndef l_lock_h
#define l_lock_h

static void create();
mixed direct_lock_obj_with_obj(object target, object tool, string id);
mixed direct_pick_str_on_obj(string str, object target, string str2,
			     string id);
mixed direct_pick_str_on_obj_with_obj(string str, object target, object tool,
				      string str2, string targ_id);
mixed direct_unlock_obj_with_obj(object target, object key, string id);

mixed CanLock(object who, string id);
mixed CanOpen(object who, string id);
mixed CanPick(object who, string id);
mixed CanUnlock(object who, string id);

mixed eventLock(object who, string id, object key);
varargs mixed eventOpen(object who, string id, object tool);
varargs mixed eventPick(object who, string id, object tool);
mixed eventUnlock(object who, string id, object key);

varargs string *SetKeys(mixed *args...);
string *AddKey(string key);
varargs string *GetKeys(string unused);
int SetLocked(int x);
int GetLocked();
int SetLockStrength(int x);
int GetLockStrength();
function SetPick(function f);
function GetPick();

#endif /* l_lock_h */
