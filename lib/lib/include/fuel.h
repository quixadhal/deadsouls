#ifndef l_fuel_h
#define l_fuel_h

static void create();

mixed eventDecreaseFuel(int x);
mixed eventRefuel(int x);

int GetFuelAmount();
static int SetFuelAmount(int x);
string GetFuelType();
static string SetFuelName(string str);
varargs string GetLong(string val);
int GetMaxFuel();
static int SetMaxFuel(int x);
int GetRefuelable();
static int SetRefuelable(int x);

/* pure virtual */ string GetShort();

#endif /* l_fuel_h */
