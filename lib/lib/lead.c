/*    From the Dead Souls Object Library
 *    Provides lead support for mobile objects.
 *    Works closely with LIB_FOLLOW.
 *    Created by Rush 951028
 */

#include <lib.h>
#include <position.h>
#include "include/lead.h"

private static mapping Followers;
private static int numberOfFollowers;

static void create(){
    Followers = ([]);
    numberOfFollowers = 0;
}

mixed direct_lead_liv(){
    if( this_object() == this_player() ) return "That's silly.";
    return this_player()->CanLead();
}

mixed direct_evade_liv(){
    if( this_object() == this_player() ) return "That's silly.";
    return this_player()->CanEvade(this_object());
}

object *AddFollower(object follower){
    mapping tmp = ([]);

    if( !objectp(follower) ) error("Bad argument 1 to AddFollower().\n");
    if( follower != this_object() && !Followers[follower]
            && !follower->GetProperty("no follow") ){
        if( !follower->IsFollowing(this_object()) ){
            if( follower->SetLeader(this_object()) ){
                tmp["followed"] = 0;
                tmp["bonus"] = 0;
                tmp["lost"] = 0;
                Followers[follower] = tmp;
                numberOfFollowers++;
            }
        }
    }
    return GetFollowers();
}

object *RemoveFollower(object follower){
    if( !objectp(follower) ) error("Bad argument 1 to RemoveFollower().\n");
    map_delete(Followers, follower);
    follower->SetLeader(0);
    numberOfFollowers = sizeof(GetFollowers());
    return GetFollowers();
}

object *GetFollowers(){ return filter(keys(Followers), (: $1 :)); }

mapping GetFollowerMap(){ return copy(Followers); }

int SetFollowed(object follower, int followed){
    if( !objectp(follower) ) error("Bad argument 1 to SetFollowed().\n");
    if( !intp(followed) ) error("Bad argument 2 to SetFollowed().\n");
    if( !Followers[follower] ) return 0;
    return(Followers[follower]["followed"] = followed);
}

int GetFollowed(object follower){
    if( !objectp(follower) ) error("Bad argument 1 to GetFollowBonus().\n");
    if( !Followers[follower] ) return 0;
    return Followers[follower]["followed"];
}

int AddFollowBonus(object follower, int bonus){
    if( !objectp(follower) ) error("Bad argument 1 to AddFollowBonus().\n");
    if( !intp(bonus) ) error("Bad argument 2 to AddFollowBonus().\n");
    if( !Followers[follower] ) return 0;
    return( Followers[follower]["bonus"] += bonus );
}

int GetFollowBonus(object follower){
    if( !objectp(follower) ) error("Bad argument 1 to GetFollowBonus().\n");
    if( !Followers[follower] ) return 0;
    return Followers[follower]["bonus"];
}

varargs mixed CanLead(object ob){
    if( ob ){
        if( !ob->CanFollow() )
            return "You are not empowered to lead " + ob->GetName();
        if( ob->IsFollowing(this_object()) )
            return "You are already leading " + ob->GetName();
        if( this_object()->IsFollowing(ob) )
            return ob->GetName() + " is already leading you.";
        if( ob == this_object() )
            return "You cannot possibly do that.";
    }
    return 1;
}

    varargs mixed CanEvade(object ob){
        if( ob && !ob->IsFollowing(this_object()) )
            return ob->GetName() + " is not following you.";
        return 1;
    }

int eventMoveFollowers(object dest){
    mapping follower;
    object ob;
    int followChance;
    int badpos;

    badpos = (POSITION_NULL|POSITION_SITTING|POSITION_LYING|POSITION_KNEELING);

    foreach(ob in GetFollowers()){
        int pos = ob->GetPosition();
        follower = Followers[ob];
        if(ob->GetSleeping() || ob->GetParalyzed() || pos & badpos
                || this_object()->GetInvis() ){
            eventEvade(ob);
            RemoveFollower(ob);
            continue;
        }
        followChance = 100;
        if( !follower["followed"] ) followChance -= 20 + this_object()->GetSkillLevel("stealth");
        followChance += ob->GetSkillLevel("tracking");
        followChance += follower["bonus"];
        if( ob->eventFollow(dest, followChance) ) follower["lost"] = 0;
        else if( follower["lost"]++ && eventEvade(ob) ){
            RemoveFollower(ob);
        }
    }
    return 1;
}

int eventEvade(object ob){
    mixed ret;
    ret = CanEvade(ob);
    if( stringp(ret) ) error(ret);
    if( ret = 1 ){
        ob->eventPrint(this_object()->GetName() + " has evaded you.");	this_object()->eventPrint("You have evaded " + ob->GetName() + ".");
        return 1;
    }
    return 0;
}
