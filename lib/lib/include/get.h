#ifndef l_get_h
#define l_get_h

static void create();
mixed direct_get_obj(object target);
mixed direct_get_obj_from_obj(object target, object src);
mixed direct_get_obj_out_of_obj(object target, object src);

mixed CanGet(object who);

mixed SetPreventGet(mixed val);
mixed GetPreventGet();

/* pure virtual */ int eventMove(mixed dest);
/* pure virtual */ int GetMass();
/* pure virtual */ mixed GetProperty(string prop);
/* pure virtual */ string GetShort();

#endif /* l_get_h */
