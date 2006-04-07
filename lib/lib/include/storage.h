#ifndef l_storage_h
#define l_storage_h

static void create();
varargs void reset(int count);

mixed direct_close_obj(object ob, string id);
mixed indirect_get_obj_out_of_obj(object item);
mixed direct_look_inside_obj(object target, string id);
mixed indirect_look_at_obj_word_obj(object target, string wrd, object ob,
  string id, string my_id);
int direct_unlock();
int inventory_accessible();
int inventory_visible();

mixed CanClose(object who, string id);
mixed CanOpen(object who, string id);
mixed CanPick(object who, string id);
mixed CanPutIn(object who, object what);
mixed CanUnlock(object who, string id, object key);

int eventReceiveObject();

int SetCanClose(int x);
int GetCanClose();
int SetCanLock(int x);
int GetCanLock();
void SetKey(string str);
varargs string GetLong(string str);
int GetOpacity();
int GetRadiantLight(int ambient);
static mixed *AddSave(mixed *val);
int SetSaveRecurse(int x);

#endif /* l_storage_h */
