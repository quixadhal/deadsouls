#include <message_class.h>
#include <daemons.h>
#include <rooms.h>
#include <terrain_types.h>
#include <position.h>
#include <medium.h>

#ifndef MAX_INVENTORY_SIZE
#define MAX_INVENTORY_SIZE 2048
#endif

private static object LastEnvironment = 0;
int anchored;

varargs int eventPrint(string str, mixed args...);
varargs void eventAnnounceCanonicalId(object env);

object GetLastEnvironment(){
    return LastEnvironment;
}

int GetAnchored(){
    return anchored;
}

int SetAnchored(int x){
    object prev = this_player();
    if(!prev || (prev != this_object() && !adminp(prev))) return anchored;
    if(x > 0) anchored = ((anchored != 2) ? (adminp(prev) ? 2 : 1) : 2);
    else anchored = ((anchored == 2) ? (adminp(prev) ? 0 : 2) : 0);
    return anchored;
}

int eventMove(mixed dest){
    object ob,me,furn,prev, env = environment();
    int depth;
    mixed tmp, ret;

    if(!interactive(this_object()) &&
            !valid_event(previous_object(), this_object())) return 0;

    if(anchored && living()){
        if(env){
            int ok;
            string tmpdest;
            string myvoid = ROOMS_D->GetVoid(this_object());
            string *dests = ({ ROOM_FREEZER, ROOM_DEATH, ROOM_VOID, myvoid });
            string *envs = dests + ({ ROOM_POD, ROOM_FURNACE });
            if(objectp(dest)) tmpdest = base_name(dest);
            else tmpdest = dest;
            if(member_array(tmpdest, dests) != -1) ok = 1;
            else if(member_array(base_name(env), envs) != -1) ok = 1;
            if(this_player() && adminp(this_player())) ok = 1;
            if(!ok){
                if(this_player()){ 
                    if(this_player() == this_object()){
                        tell_object(this_object(), "You are anchored here.");
                    }
                    else {
                        tell_object(this_object(), this_player()->GetName()+
                                " tried to move you but you are anchored here.");
                    }
                }
                return 0;
            }
            else tell_object(this_object(), "You are about to be moved "+
                    "to "+identify(dest));
        }
    }

    me = this_object();

    if( !me ){
        return 0;
    }
    if( environment() ){
        int x;

        x = environment()->CanRelease(me);
        if( !x && !adminp() ){
            return 0;
        }
    }
    if( stringp(dest) ){
        if( !(ob = find_object(dest)) ){
            string str;

            if( str = catch(call_other(dest, "???")) ){
                if( creatorp() ){
                    eventPrint(str, MSG_ERROR);
                }
                return 0;
            }
            ob = find_object(dest);
        }
    }
    else {
        ob = dest;
    }
    if( !ob || ob == me ){
        return 0;
    }
    if( !ob->CanReceive(me) ){
        return 0;
    }
    if(sizeof(deep_inventory(ob)) > MAX_INVENTORY_SIZE){
        return 0;
    }
    if( objectp(me) && LastEnvironment = environment() ){
        environment()->eventReleaseObject(me);
        if(!undefinedp(me->GetRecurseDepth()) && 
                !undefinedp(environment()->GetRecurseDepth())){
            depth = me->GetRecurseDepth();
            depth += 1;
            if(depth) environment()->AddRecurseDepth(-depth);
            if(environment()->GetRecurseDepth() < 1) environment()->SetRecurseDepth(1);
        }
    }
    if(!objectp(me) ){
        return 0; 
    }
    prev = environment(me);
    move_object(ob);
    tmp = ob->eventReceiveObject(me);
    if(environment() == prev){
        return 0;
    }
    if(prev) prev->eventPostRelease(me);
    if( environment() ){
#if 0
        foreach(object peer in all_inventory(environment())){
            if( peer != me ){
                catch(peer->eventEncounter(me));
            }
        }
#endif
        if(OBJECT_MATCHING){ 
            object *prev_inv = ({});
            eventAnnounceCanonicalId();
            if(prev && sizeof(prev_inv = deep_inventory(prev)))
                prev_inv->ReceiveCanonicalId(ob->GetCanonicalId(), 1);
        }
    }
    if(living(me) && furn = me->GetProperty("furniture_object"))
        if(objectp(furn)) furn->eventReleaseStand(me);

    if(living(me) && !living(environment(me)) 
            && furn = me->GetProperty("mount"))
        if(objectp(furn)){
            me->RemoveProperty("mount");
            furn->eventDismount(me);
        }
    if(environment()->GetMedium() == MEDIUM_AIR &&
            me->GetPosition() != POSITION_FLYING){
        if(!(me->CanFly())){
            if(!interactive(me)){
                call_out("eventFall", 1);
            }
        }
        else me->eventFly();
    }
    if(environment()->GetMedium() == MEDIUM_LAND &&
            (me->GetPosition() == POSITION_FLOATING ||
             me->GetPosition() == POSITION_SWIMMING)){
        call_out("eventCollapse", 1, 1);
    }
    else if((environment()->GetMedium() == MEDIUM_WATER ||
                environment()->GetMedium() == MEDIUM_SURFACE) &&
            me->GetPosition() != POSITION_SWIMMING &&
            me->GetPosition() != POSITION_FLOATING){
        if(living(me)){
            if(!(me->GetPosition() == POSITION_FLYING &&
                        environment()->GetMedium() == MEDIUM_SURFACE)){
                if(!(environment()->GetTerrainType() & (T_SEAFLOOR)) && me->CanSwim()){
                    if(me->GetPosition() != POSITION_FLOATING){
                        me->eventSwim();
                    }
                }
                else {
                    call_out("eventSink", 1);
                }
            }
        }
        else {
            if(!me->GetProperty("buoyant")){
                call_out("eventSink", 1);
            }
        }
    }
    ret = (LastEnvironment != environment());
    if(ret && living(this_object()) && living(environment())){
        if(!interactive(this_object()) &&
                !this_object()->GetBefriended(environment())){
            if(query_verb() == "get" || query_verb() == "take"){
                call_out("SetAttack", 0, environment());
            }
        }
    }
    if(environment() && !(environment()->GetZPEM())){
        this_object()->SetZPG(0);
    }
    return ret;
}
