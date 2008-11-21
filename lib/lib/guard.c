#include <daemons.h>
#include <position.h>

private static mixed GuardAction;
private static mixed globalwhat;
private static array PendingGuard = ({});
private static object gwhat;
private static object Principal;

int AllowPass(object who, object what){
    string race = this_object()->GetRace();
    if(who == this_object()) return 1;
    if(who->GetInvis()) return 1;
    if(this_object()->GetSleeping()) return 1;
    if(this_object()->GetParalyzed()) return 1;
    if(this_object()->GetPosition() == POSITION_LYING &&
      !(RACES_D->GetLimblessCombatRace(race)) ) return 1; 
    return 0;
}

int AllowGet(object who, object what){
    string race = this_object()->GetRace();
    if(who == this_object()) return 1;
    if(who->GetInvis()) return 1;
    if(this_object()->GetSleeping()) return 1;
    if(this_object()->GetParalyzed()) return 1;
    if(this_object()->GetPosition() == POSITION_LYING &&
      !(RACES_D->GetLimblessCombatRace(race)) ) return 1;
    return 0;
}

varargs mixed SetGuard(mixed what, mixed action, int howlong){
    object env = environment();
    if(!clonep(this_object())) return 0;
    if(!what && !sizeof(PendingGuard)) return 0;
    if(!PendingGuard) PendingGuard = ({});

    PendingGuard += ({ ([ "what" : what, "action" : action,
        "howlong" : howlong ]) });

    if(!env){
        return 0;
    }

    if(stringp(what)){
        int err;
        mapping exits = env->GetExitMap();
        exits += env->GetEnterMap();
        gwhat = what;
        if(exits[gwhat]){
            gwhat = exits[gwhat];
        }
        if(!unguarded( (: directory_exists(path_prefix(gwhat)) :) ) ){
            return 0;
        }
        err = catch( unguarded( (: gwhat = load_object(gwhat) :) ) );
        if(err || !gwhat){
            return 0;
        }
    }

    if(living(gwhat)){
        Principal = gwhat;
        this_object()->eventForce("follow "+gwhat->GetKeyName());
    }

    else if(!inherits(LIB_ROOM,gwhat)){
        mixed inv = deep_inventory(env);
        inv = filter(inv, (: base_name($1) == base_name(gwhat) :)); 
        foreach(object ob in inv){
            GUARD_D->AddGuard(this_object(), ob, action);
            if(living(ob)){
                Principal = ob;
            }
        }
    }
    else GUARD_D->AddGuard(this_object(), gwhat, action);
    gwhat = 0;
    return 1;
}

void CheckPending(){
    if(PendingGuard && clonep() && sizeof(PendingGuard)){
        mixed tmp_pending = ({});
        int ret;
        foreach(mixed guardmount in PendingGuard){
            ret = SetGuard(guardmount["what"], guardmount["action"], 
              guardmount["howlong"]);
            if(!ret) tmp_pending += ({ guardmount });
        }
        PendingGuard = tmp_pending;
    }
}

void heart_beat(){
    if(Principal){
        object enemies = Principal->GetEnemies();
        if(sizeof(enemies)){
            foreach(object ob in enemies){
                if(member_array(ob, this_object()->GetEnemies()) == -1){
                    this_object()->AddEnemy(ob);
                }
            }
        }
    }
}
