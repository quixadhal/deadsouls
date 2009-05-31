#include <daemons.h>
#include <position.h>

private static mixed GuardAction;
private static mixed globalwhat;
private static array PendingGuard = ({});
private static mixed gwhat;
private static object Principal;
private static mapping GuardMap = ([]);

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
    if(!GuardMap) GuardMap = ([]);
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
        if(exits && exits[gwhat]){
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
    if(gwhat && living(gwhat)){
        Principal = gwhat;
        this_object()->eventForce("follow "+gwhat->GetKeyName());
    }

    else if(gwhat && !inherits(LIB_ROOM,gwhat)){
        mixed inv = deep_inventory(env);
        inv = filter(inv, (: base_name($1) == base_name(gwhat) :)); 
        inv = filter(inv, (: $1 && !interactive(environment($1)) :)); 
        if(!sizeof(inv)){ 
            if(objectp(gwhat)) what = base_name(gwhat);
            GuardMap[random(20)] = ([ "base" : what, "action" : action ]);
        }
        foreach(object ob in inv){
            GUARD_D->AddGuard(this_object(), ob, action);
            GuardMap[file_name(ob)] = ([ "base" : base_name(ob),
                    "action" : action ]);
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

void CheckGuardeds(){
    if(!GuardMap) GuardMap = ([]);
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
    if(sizeof(GuardMap)){
        mapping RetMap = ([]), ReGuard = ([]);
        foreach(string key, mapping tmpmap in GuardMap){
            object ob, env;
            if(!(ob = find_object(key)) || !(env = environment(ob)) ||
                    env != environment(this_object())){
                ReGuard[tmpmap["base"]] = tmpmap["action"];
            }
            else RetMap[key] = tmpmap;
        }
        GuardMap = copy(RetMap);
        if(sizeof(ReGuard)){
            foreach(string key, mixed action in ReGuard){
                SetGuard(key, action);
            }
        }
    }
}

void heart_beat(){
    CheckGuardeds();
}

void init(){
    CheckGuardeds();
    CheckPending();
}
