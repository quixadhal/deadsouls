#ifndef l_meal_h
#define l_meal_h

static void create();
mixed direct_drink_obj();
mixed direct_drink_from_obj();
mixed direct_eat_obj();

mixed eventDrink(object who);
mixed eventEat(object who);
mixed eventPoison(object who, object agent, int x);

string SetEmptyItem(string file);
string GetEmptyItem();
string SetEmptyName(string str);
string GetEmptyName();
mixed SetEmptyShort(mixed str);
mixed GetEmptyShort();
mixed SetEmptyLong(mixed str);
mixed GetEmptyLong();
int SetMealType(int x);
int GetMealType();
int SetStrength(int x);
int GetStrength();
varargs void SetMealMessages(mixed array val...);
mixed *GetMessages();
int GetValue();

#endif /* l_meal_h */
