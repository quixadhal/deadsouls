#ifndef l_leader_h
#define l_leader_h

static void create();
mixed eventAsk(object who, string what);
void eventConvert(object who, string args);
void eventPreview(object who, string args);
void eventJoin(object who, string args);
int eventPreAttack(object ob);

#endif /* l_leader_h */
