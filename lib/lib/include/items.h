#ifndef l_items_h
#define l_items_h

static void create();
mixed direct_look_at_str_on_obj(string str, object target);

varargs mixed eventShow(object who, string component);

mapping SetItems(mapping items);
mixed AddItem(mixed item, mixed val);
mapping RemoveItem(mixed item);
varargs mixed GetLong(string item);

/* pure virtual */ string GetShort();

#endif /* l_items_h */
