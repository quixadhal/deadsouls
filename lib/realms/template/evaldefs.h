#ifndef TP
#define TP this_player()
#endif
#ifndef ENV
#define ENV environment(this_player())
#endif
#ifndef HERE
#define HERE room_environment(this_player())
#endif
#define THING(x) (get_object(x))
#define PRES(x) (present(x, TP) || present(x, ENV) )
#define FILE(x) (present_file(x, TP) || present_file(x, ENV) )
