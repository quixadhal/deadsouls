#ifndef l_lead_h
#define l_lead_h

static void create();
object *AddFollower(object follower);
object *RemoveFollower(object follower);
object *GetFollowers();

int SetFollowed(object follower, int followed);
int GetFollowed(object follower);
int AddFollowBonus(object follower, int bonus);
int GetFollowBonus(object follower);

varargs mixed CanLead(object ob);
varargs mixed CanEvade(object ob);

int eventMoveFollowers(object dest);
int eventEvade(object ob);


#endif /* l_lead_h */


