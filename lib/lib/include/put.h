#ifndef l_put_h
#define l_put_h

static void create();
mixed direct_put_obj_word_obj();

mixed CanPut(object who);

mixed eventPut(object who, object storage);

mixed SetPreventPut(mixed val);
mixed GetPreventPut();

/* pure virtual */ mixed CanDrop(object who);
/* pure virtual */ int eventMove(mixed dest);
/* pure virtual */ string GetShort();

#endif /* l_put_h */
