#ifndef l_drop_h
#define l_drop_h

static void create();
mixed direct_drop_obj(object target);

mixed CanDrop(object who);

mixed eventDrop(object who);

mixed SetPreventDrop(mixed val);
mixed GetPreventDrop();

/* pure virtual */ int eventMove(mixed dest);
/* pure virtual */ string GetShort();

#endif /* l_drop_h */
