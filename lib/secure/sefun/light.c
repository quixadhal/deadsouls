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
    if( creatorp(ob) || newbiep(ob) ) return 1;
    x = total_light(ob) + (int)ob->GetSightBonus();
    i = sizeof(inv = all_inventory(ob));
    while(i--) x += (int)inv[i]->GetProperty("light");
    return x;
}

varargs int total_light(object ob) {
    object *inv;
    object env;
    int i, x;

    if( !ob ) ob = previous_object();
    if( !(env = environment(ob)) ) return 0;
    i = sizeof(inv = all_inventory(env));
    x = (int)env->GetProperty("light");
    while(i--) x += (int)inv[i]->GetProperty("light");
    if( (int)env->GetProperty("indoors") ) return x;
    switch((string)SEASONS_D->query_time_of_day()) {
        case "day": return x;
        case "night":
	  x += (int)SEASONS_D->GetMoonLight();
          x += (int)env->GetProperty("night light") - 4;
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
