#ifndef l_steal_h
#define l_steal_h

static void create();
mixed direct_steal_obj_from_liv();

mixed CanSteal(object who);

mixed eventSteal(object who);

mixed SetPreventSteal(mixed val);
mixed GetPreventSteal();

/* pure virtual */ mixed eventMove(mixed dest);
/* pure virtual */ int GetInvis(object who);
/* pure virtual */ string GetShort();

#endif /* l_steal_h */
