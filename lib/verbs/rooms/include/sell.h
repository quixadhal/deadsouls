#ifndef l_sell_h
#define l_sell_h

static void create();
mixed can_sell();
mixed can_sell_obj_to_liv(object ob, object vendor);
mixed do_sell();
mixed do_sell_obj_to_liv(object ob, object vendor);
string GetHelp(string str);

#endif /* l_sell_h */
