#ifndef l_close_h
#define l_close_h

static void create();
int inventory_accessible();
int inventory_visible();
mixed direct_close_obj(object target, string id);
mixed direct_open_obj(object target, string id);
mixed direct_open_obj_with_obj(object target, object tool, string id);

mixed CanClose(object who, string id);
mixed CanOpen(object who, string id);

mixed eventClose(object who, string id);
varargs mixed eventOpen(object who, string id, object tool);

int SetClosed(int x);
int GetClosed();

/* pure virtual */ static mixed *AddSave(mixed *vars);
/* pure virtual */ string GetShort();

#endif /* l_close_h */
