#ifndef l_press_h
#define l_press_h

static void create();
mixed direct_press_obj(object target);
mixed direct_press_str_on_obj(string str, object target);

varargs mixed CanPress(object who, string component);
varargs mixed eventPress(object who, string component);

varargs mapping SetPress(mixed key, mixed desc);
mapping RemovePress(string item);
mixed GetPress(string str);
string array GetPresses();

/* pure virtual */ string GetShort();

#endif /* l_press_h */
