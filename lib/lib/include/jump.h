#ifndef l_jump_h
#define l_jump_h

static void create();

mixed direct_jump_word_obj(string prep, object target, string id);
varargs mixed direct_jump_word_word_obj(mixed args...);

mixed CanJump(object who, string id, int type);

mixed eventJump(object who, string id, int type);

mapping SetJump(function f, int type);
varargs mapping AddJump(string dir, mixed dest, int type);
mixed GetJumpDestination(string dir);
int *GetJumpType(string dir);
string *GetJumps();

/* pure virtual */ string *GetId();

#endif /* l_jump_h */
