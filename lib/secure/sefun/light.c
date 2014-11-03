/*    /adm/simul_efun/light.c
 *    from Dead Souls
 *    light sefuns
 *    created by Descartes of Borg 940213
 */

#include <daemons.h>

varargs int effective_light(object ob) {
    object *inv;
    int i, x;

    if( !ob ) ob = previous_object();
    x = total_light(ob) + ob->GetSightBonus();
    i = sizeof(inv = all_inventory(ob));
    while(i--) x += inv[i]->GetRadiantLight();
    return x;
}

varargs int total_light(object ob) {
    object *inv;
    object env;
    int i, x;

    if( !ob ) ob = previous_object();
    if( !(env = environment(ob)) ) return 0;
    i = sizeof(inv = all_inventory(env));
    x = env->GetAmbientLight();
    x += ob->GetRadiantLight();
    while(i--) x += inv[i]->GetRadiantLight();
    if( env->GetClimate() == "indoors" ) return x;
    switch(SEASONS_D->query_time_of_day()) {
        case "day": return x;
        case "night":
                    x += SEASONS_D->GetMoonLight();
                    return x;
        case "dawn": case "twilight": return (x-1);
        default: return x;
    }
}

varargs int visibility(object ob) {
    int x;

    if( !ob ) ob = previous_object();
    x = effective_light(ob);
    if( x > 5 || x < -2 ) return 0;
    else if( x > 3 || x < 0 ) return 1;
    else return 2;
}

mixed check_light(object who) {
    int light;
    object env;
    if(!who) who = this_player();
    env = environment(who);
    if(!env) return "You are nowhere.";
    if(env->GetMount() || base_name(env) == LIB_CORPSE){
        env = environment(environment(this_player()));
        if(!env){
            return "You are in serious trouble. Ask an admin for help.";
        }
    }
    light = who->GetEffectiveVision(env);
    if( light < 3 ) {
        return "It's too dark to see.";
    }
    else if( light > 6 ) {
        return "It's too bright to see.";
    }
    else {
        return 1;
    }
}

