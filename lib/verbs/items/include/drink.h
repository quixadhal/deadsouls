#ifndef l_drink_h
#define l_drink_h

static void create();

mixed can_drink_obj(string verb);
mixed can_drink_from_obj(string verb);

mixed do_drink_obj(object ob);
mixed do_drink_from_obj(object ob, string id);

varargs void eventDrink(object who, object what, string id);

#endif /* l_drink_h */
