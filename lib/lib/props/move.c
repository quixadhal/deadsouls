#include <message_class.h>
#include <terrain_types.h>
#include <position.h>
#include <medium.h>

#ifndef MAX_INVENTORY_SIZE
#define MAX_INVENTORY_SIZE 2048
#endif

private static object LastEnvironment = 0;

varargs int eventPrint(string str, mixed args...);
varargs void eventAnnounceCanonicalId(object env);

object GetLastEnvironment(){
    return LastEnvironment;
}

int eventMove(mixed dest){
    object ob,me,furn,prev;
    int depth;
    mixed tmp, ret;
    me = this_object();

    if( !me ){
        return 0;
    }
    if( environment() ){
        int x;

        x = (int)environment()->CanRelease(me);
        if( !x && !archp() ){
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
        foreach(object peer in all_inventory(environment())){
            if( peer != me ){
                catch(peer->eventEncounter(me));
            }
        }
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
            call_out("eventFall", 1);
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
    return ret;
}
