#include <daemons.h>

int user_exists(string str){ 
    return master()->player_exists(str); 
}

string user_path(string name){
    return PLAYERS_D->GetUserPath(name);
}

string *remote_users(){
    mixed *insts = INSTANCES_D->GetInstances();
    mixed tmp, *ret = ({});
    foreach(mixed inst in insts){
        if(inst) tmp = INSTANCES_D->GetRemoteUsers(inst);
        if(sizeof(tmp)){
            if(!arrayp(tmp))  ret += ({ tmp });
            else ret += tmp;
        }
    }
    return ret;
}

string *local_users(){
    string *ret;
    ret = map(filter(users(),(: (environment($1) && !((int)$1->GetInvis())) :)),
            (: (string)$1->GetKeyName():));
    return ret;
}
