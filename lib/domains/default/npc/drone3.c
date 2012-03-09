/* A more thorough world mapping bot. */

#include <lib.h>
#include ROOMS_H
#include <position.h>
#include <medium.h>
#include <daemons.h>

inherit LIB_SENTIENT;

int spawned;
int hb = 10;

string ShortDesc(){
    return "drone "+file_name();
}

string LongDesc(){
    return "drone "+file_name();
}

static void create() {
    sentient::create();
    SetKeyName("drone");
    SetAdjectives( ({"small","metal"}) );
    SetId( ({"drone","bot","robot"}) );
    SetShort( (: ShortDesc :) );
    SetLong( (: LongDesc :) );
    SetLevel(1);
    SetInvis(1);
    SetRace("bot");
    SetGender("neuter");
    SetClass("explorer");
    SetMaxHealthPoints(50000);
    SetPermitLoad(1);
    SetEncounter(0);
    SetPosition(POSITION_FLYING);
    SetNoClean(1);
    SetPacifist(1);
    SetWimpy(99);
    set_heart_beat(hb);
}

int eventReceiveDamage(mixed args...){
    return 0;
}

int eventMove(mixed dest){
    int ret;
    object env = environment();     
    string location;

    if(!env) location = ROOM_START;     
    else if(clonep(env)) location = file_name(env);
    else location = base_name(env);

    if(location) this_object()->SetProperty("LastLocation", location); 
    ret = ::eventMove(dest);
    AddStaminaPoints(GetMaxStaminaPoints());
    if(env){
        if(env->GetMedium() != MEDIUM_LAND) spawned = 1;
        if(env->GetVirtual()){
            hb = 20;
            set_heart_beat(hb);
        }
        MAP_D->GetMap(env, 8, 1);
    }
    return ret;
}

void init(){
    ::init();
    set_heart_beat(hb);
}

void eventSpawn(){
    object env;
    string location;
    string *exits = ({});
    object newdrone;

    if(!env = environment()) return;
    spawned = 1;

    exits = values(env->GetExitMap());
    exits += values(env->GetEnterMap());
    exits += (({env->GetFlyRoom()}));
    exits += (({env->GetSinkRoom()}));
    exits = distinct_array(exits);

    foreach(string exit in exits){
        if(member_array(exit, ROOMS_D->DroneCache()) != -1) continue;
        newdrone = new("/domains/default/npc/drone3");
        newdrone->eventMove(env);
#if 0
        drones = objects( (: base_name($1) == base_name() &&
                    environment($1) && !strsrch(base_name(environment($1)),
                        path_prefix(base_name(environment()))) && $1 != this_object() :) );
        if(drones) drones->ReceiveNoGo(nogo + no_go + ({ base_name(env) })); 
#endif
        ROOMS_D->DroneCache( ({ exit }) );
        if(strsrch(exit, "/virtual/") < 0){
            catch( newdrone->eventMove(exit) );
        }
    }
    ROOMS_D->DroneCache(({ base_name(env) }));
} 

void heart_beat(){
    if(!clonep()) return;
    if(!spawned) eventSpawn();
    else {
        if(environment()) MAP_D->GetMap(environment(), 8, 1);
        eventDestruct();
    }
}
