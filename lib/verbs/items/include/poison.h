#ifndef l_poison_h
#define l_poison_h
 
static void create();

mixed can_poison_obj_with_obj(string verb);

mixed do_poison_obj_with_obj(object target, object agent);

void eventPoison(object who, object target, object agent);
 
#endif /* l_poison_h */
