/*    From the Dead Soulsr2 Object Library
 *    Provides follow/trail support for mobile objects.
 *    Works closely with LIB_LEAD.
 *    Created by Rush 951028
 */

#include <lib.h>
#include "include/follow.h"

private static object Leader;

static void create() { Leader = 0; }

mixed direct_follow_liv() {
    if( this_object() == this_player() )
      return "It doesn't quite work that way.";
    return this_player()->CanFollow(this_object());
}

object SetLeader(object leader) {
    if( leader == this_object() ) return 0;
    if( leader && Leader ) return 0;
    Leader = ( leader && leader->GetFollowers() ) ? leader : 0;
    return Leader;
}

object GetLeader() { return Leader; }

varargs mixed CanFollow(object ob) {
    if( ob ) {
        if( !ob->CanLead() )
            return "You are not empowered to follow " + ob->GetName();
        if( IsFollowing(ob) )
            return "You are already following " + ob->GetName();
        if( ob->IsFollowing(this_object()) )
            return ob->GetName() + " is already following you.";
        if( ob == this_object() )
            return "You cannot possibly do that.";
    }
    return 1;
}

int IsFollowing(object ob) {
    if( !Leader ) return 0;
    if( Leader == ob ) return 1;
    if( Leader->IsFollowing(ob) ) return 1;
    return 0;
}

// eventFollow() needs to be overriden by child classes to provide the
// actual move support.
int eventFollow(object dest, int followChance) { return 0; }


