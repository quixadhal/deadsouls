/*    /lib/comp/seal.c
 *    from the Dead Souls Mud Library
 *    Composite component of a closeable and lockable thing
 *    Created by Descartes of Borg 961221
 *    Version: @(#) seal.c 1.2@(#)
 *    Last modified: 96/12/23
 */

#include <lib.h>

inherit LIB_CLOSE;
inherit LIB_LOCK;

mixed CanLock(object who, string id){
    mixed tmp = lock::CanLock(who);

    if( tmp != 1 ){
        return tmp;
    }
    if( !GetClosed() ){
        return "You cannot lock it while it is open.";
    }
    return 1;
}

varargs mixed CanOpen(object who, string id){
    if( GetLocked() ){
        id = "It is locked!";
        return id;
    }
    else return close::CanOpen(who);
}

varargs mixed eventOpen(object who, object tool){
    if( tool && GetLocked() ){
        mixed tmp;
        if(tool && objectp(tool)){
            tmp =  eventPick(who, tool->GetKeyName(), tool);
        }
        else tmp = eventPick(who);

        if( tmp != 1 || GetLocked() ){
            return tmp;
        }
    }
    if( GetLocked() ){
        send_messages(({ "attempt", "find" }), "$agent_name $agent_verb to "
                "open $target_name, but $agent_nominative $agent_verb "
                "it locked.", who, this_object(), environment(who));
        return 1;
    }
    return close::eventOpen(who, tool);
}

void create(){
}
