/* Hiccups@Frontiers, 12/16/96 */
/* Fixes by Haderach, 05 SEP 2005 */
/* RWHO check by Shadyman, 2006-Sep-12 */

#include <lib.h>
#include <privs.h>

inherit LIB_DAEMON;

#define SEP repeat_string("*=",37)+"*<br>";

varargs string gateway(int strip_html) {
    int p;
    string x, tmp="", ret="";
    object *obs;

    p = 0;
    obs=users();

    for (int i=0; i<sizeof(users()); i++) {
        if(!obs[i] || !environment(obs[i])) continue;
        if(obs[i]->GetKeyName() != last_string_element(base_name(obs[i]),"/")) continue;
        if(!obs[i]->GetInvis()) {
            if(archp(obs[i])) tmp+="[ARCH]";
            else if(creatorp(obs[i]) ) tmp+="[WIZ]";
            else if(avatarp(obs[i]) ) tmp+="[AVATAR]";
            else if(high_mortalp(obs[i]) ) tmp+="[HIGH MORTAL]";
            else tmp+=sprintf("[%d]", obs[i]->GetLevel() );
            if(elderp(obs[i])) tmp+="[ELDER]";
            if(ambassadorp(obs[i])) tmp+="[AMBASSADOR]";
            tmp += ": ";
            if(sizeof(obs[i]->GetShort()) < 50) {
                tmp+=sprintf(" %s", obs[i]->GetShort());
            }
            else {
                tmp+=" "+capitalize(obs[i]->GetKeyName())+ " the Long-Titled.";
            }
            if(obs[i]->GetSleeping() > 0) tmp+=" (sleeping) ";
            else if (obs[i]->GetProperty("afk")) tmp+=" (afk)";
            else if (query_idle(obs[i])>240 && obs[i]->GetInCombat()!=1)  tmp+=" (idle)";
            else if (in_edit(obs[i])) tmp+=" (edit)";
            else if(obs[i]->GetInCombat())  tmp+=" (combat)";
            tmp+="<br>";
            p++;
        }
    }
    if(!strip_html){
        ret+="<center>"+mud_name()+"</center><br>";
    }
    else {
        ret+=mud_name()+"\n";
    }
    ret+=SEP;
    ret+=tmp;
    ret+=SEP;
    x="There ";
    (p==1) ? x+="is " : x+="are ";
    x+=cardinal(p);
    (p==1) ? x+=" member " : x+=" members ";
    x+="of our reality.<br>";
    ret+=center(x);
    if(strip_html){
        ret = replace_string(ret,"<br>","\n");
    }
    return ret;
}
