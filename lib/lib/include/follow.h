#ifndef l_follow_h
#define l_follow_h

static void create();
object SetLeader(object leader);
object GetLeader();
varargs mixed CanFollow(object ob);
int IsFollowing(object ob);
int eventFollow(object dest, int followChance);


#endif /* l_follow_h */


