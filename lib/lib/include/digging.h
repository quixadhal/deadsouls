#ifndef l_digging_h
#define l_digging_h

static void create();
mixed indirect_bury_obj_with_obj();
mixed direct_dig_with_obj();
mixed indirect_dig_obj_with_obj(string id);

mixed eventBury(object who, string what);
varargs mixed eventDig(object who, string what);

#endif /* l_digging_h */
