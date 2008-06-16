#ifndef l_door_h
#define l_door_h

class door_side {
    int Lockable;
    string *Ids;
    mixed Short;
    mixed Long;
    string *Keys;
    object *Rooms;
}

static void create();

mixed CanLock(object who, string id);
mixed CanUnlock(object who, string id);

varargs int eventClose(object by, string id);
mixed eventLock(object who, string id, object key);
varargs int eventOpen(object who, string id, object tool);
int eventRegisterSide(string side);
mixed eventUnlock(object who, string id, object key);

void SetSide(string side, mapping mp);
int SetLockable(string side, int x);
int GetLockable(string side);
varargs string *SetId(string side, mixed *args...);
string *GetId(string side);
mixed SetShort(string side, mixed short);
varargs string GetShort(string side);
mixed SetLong(string side, mixed long);
string GetLong(string side);
varargs string *SetKeys(string side, mixed *args...);
string *GetKeys(string side);
object *GetRooms(string side);

#endif /* l_door_h */
