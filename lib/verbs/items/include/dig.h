#ifndef l_dig_h
#define l_dig_h

static void create();

mixed can_dig_with_obj(string verb);
mixed can_dig_obj_with_obj(string verb);

mixed do_dig_with_obj(object ob);
mixed do_dig_obj_with_obj(object what, object ob, string id);

void eventDig(object who, object tool, object target, string what);

#endif /* l_dig_h */
