#include <lib.h>
#include <dirs.h>
#include ROOMS_H
#include <damage_types.h>

inherit LIB_SHADOW;
int count = 300;

void heart_beat(){
    object env = room_environment(GetShadowedObject());
    if(!env) return;
    count--;
    this_object()->eventPrint("You hunger for brains.");
    if(!(count % 10) && env){
        tell_room(env,this_object()->GetName()+" shudders and groans.",
                ({ this_object() }) );
        GetShadowedObject()->eventReceiveDamage("decay",OTHER,random(50),1);
    }
    if(count < 0){
        mixed *inv = all_inventory(GetShadowedObject());
        tell_room(env,this_object()->GetName()+" falls apart!",
                ({ this_object() }) );
        if(sizeof(inv)) inv->eventMove(env);
        foreach(mixed element in this_object()->GetLimbs()){
            this_object()->RemoveLimb(element,"decay",1);
        }
        this_object()->eventMove(ROOM_FURNACE);
    }
}

int eventForce(string cmd){
    if(interactive(GetShadowedObject())) return 0;
    return GetShadowedObject()->eventForce(cmd);
}

mixed eventTalkRespond(mixed args...){
    string cmd = "";
    string *punct = ({".",",","!","?",";"});
    if(sizeof(args) > 3) cmd = args[3];
    cmd = lower_case(cmd);
    if(sizeof(cmd) && member_array(last(cmd, 1), punct) != -1){
        cmd = truncate(cmd, 1);
    }
    //No commanding players plz
    if(interactive(GetShadowedObject())) return 0;
    call_out( (: eventForce :), 1, cmd);
    return 1;
}

int eventShadow(object whom){
    if(userp(whom)) return 0;
    whom->SetEncounter(0);
    whom->SetGuard(0);
    whom->SetNoClean(1);
    set_heart_beat((query_heart_beat(whom) || 1));
    return ::eventShadow(whom);
}
