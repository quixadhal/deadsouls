#ifndef l_burn_h
#define l_burn_h

static void create();
static void heart_beat();

mixed direct_burn_obj_with_obj();
mixed indirect_burn_obj_with_obj(object target, object source);
mixed indirect_burn_obs_with_obj(object *targets, object source);
mixed direct_light_obj(object target);
mixed direct_light_obj_with_obj();
mixed indirect_light_obj_with_obj(object target, object source);
mixed indirect_light_obs_with_obj(object *targets, object source);

mixed eventBurnOut();
mixed eventBurn(object who, object what);
mixed eventLight(object who, object what);

int GetBurning();
int GetBurnRate();
static int SetBurnRate(int x);
int GetBurntValue();
static int SetBurntValue(int x);
int GetFuelRequired();
static int SetFuelRequired(int x);
int GetHeat();
static int SetHeat(int x);
int GetMinHeat();
static int SetMinHeat(int x);

/* pure virtual */ string GetShort();
/* pure virtual */ int SetValue(int x);

#endif /* l_burn_h */
