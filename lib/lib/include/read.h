#ifndef l_read_h
#define l_read_h

static void create();

mixed direct_read_obj();
mixed direct_read_str_word_obj(string str);

mixed CanRead(object who, string str);

varargs mixed SetRead(mixed arg1, mixed desc);
mapping RemoveRead(string item);
mixed GetRead(string str);
string *GetReads();

#endif /* l_read_h */
