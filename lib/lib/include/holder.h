#ifndef l_holder_h
#define l_holder_h

static void create();
varargs void reset(int count);
int inventory_accessible();
int inventory_visible();
mixed indirect_get_obj_from_obj(object item);
mixed indirect_get_obj_out_of_obj(object item);
mixed indirect_get_obs_from_obj(object *item, object storage);
mixed indirect_get_obs_out_of_obj(object *item, object storage);
mixed direct_look_in_obj(object target, string id);
mixed direct_look_inside_obj(object target, string id);
mixed indirect_look_at_obj_word_obj(object target, string wrd, object ob,
				    string id, string my_id);
mixed indirect_put_obj_word_obj(object what, string word, object storage);
mixed indirect_put_obs_word_obj(object *items, string word, object storage);

mixed CanPutIn(object who, object what);
varargs mixed CanShowInterior(object who, object what);

mixed eventGetFrom(object who, object *what);
mixed eventPutIn(object who, object what);
int eventReceiveObject();
int eventReleaseObject();
mixed eventShowInterior(object who);

mapping SetInventory(mapping inv);
mapping GetInventory();
varargs string GetLong(string str);
int SetMaxCarry(int x);
int GetMaxCarry();

/* pure virtual */ int GetInvis(object ob);
/* pure virtual */ static mixed *AddSave(mixed *vars);
/* pure virtual */ int SetSaveRecurse(int x);
/* pure virtual */ string GetShort();

#endif /* l_holder_h */
