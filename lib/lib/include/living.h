#ifndef l_living_h
#define l_living_h

static void create();
int is_living();
int inventory_accessible();
int inventory_visible();
mixed direct_give_liv_obj();
mixed indirect_give_obj_to_liv(object item);
mixed direct_give_liv_wrd_wrd(object target, string num, string curr);
mixed direct_give_wrd_wrd_to_liv(string num, string curr);
mixed direct_look_at_obj();
mixed direct_steal_wrd_from_liv(string str);
mixed indirect_steal_obj_from_liv(object item, mixed args...);

int eventFollow(object dest, int followChance);
varargs mixed CanCastMagic(int hostile, string spell);
mixed CanReceiveMagic(int hostile, string spell);

int SetPK(int x);
int GetPK();

/* pure virtual */int eventForce(string str);
/* pure virtual */ mixed GetProperty(string prop);

#endif /* l_living_h */
