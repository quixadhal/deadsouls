/* Hiccups@Frontiers, 12/16/96 */
/* Fixes by Haderach, 05 SEP 2005 */
/* RWHO check by Shadyman, 2006-Sep-12 */

#include <lib.h>
#include <privs.h>
#include <config.h>

inherit LIB_DAEMON;

#define SEP repeat_string("*=",39)+"*\n";

int cmd(string args) {
    int p;
    string x, tmp="", ret="";
    object *obs;

    if (args && args[0] == '@') { //If there's an @ in the first arg character, it's an rwho request.
        if( sizeof(args) <=1 ) return notify_fail("Get a remote who from where?"); //If there's only an @, tell them to RTFM.
        "/cmds/players/rwho"->cmd(args[1..]); //Pass it the mud name without the @
        return 1;
    } else {
        p = 0;
        obs=users();

        for (int i=0; i<sizeof(users()); i++) {
            if(!obs[i] || !environment(obs[i])) continue;
            if(obs[i]->GetKeyName() != last_string_element(base_name(obs[i]),"/")) continue;
            if(!obs[i]->GetInvis()) {
                if(archp(obs[i])) tmp+="[%^BLUE%^ARCH%^RESET%^]";
                else if(creatorp(obs[i]) ) tmp+="[%^CYAN%^WIZ%^RESET%^]";
                else if(avatarp(obs[i]) ) tmp+="[%^GREEN%^AVATAR%^RESET%^]";
                else if(high_mortalp(obs[i]) ) tmp+="[%^GREEN%^HIGH MORTAL%^RESET%^]";
                else tmp+=sprintf("[%d]", obs[i]->GetLevel() );
                if(elderp(obs[i])) tmp+="[%^YELLOW%^ELDER%^RESET%^]";
                if(testp(obs[i]) && !archp(obs[i])) tmp+="[%^B_CYAN%^%^BLACK%^TEST%^RESET%^]";
                if(ambassadorp(obs[i])) tmp+="[%^YELLOW%^AMBASSADOR%^RESET%^]";
                tmp += ": ";
                if(sizeof(strip_colours(obs[i]->GetShort())) < 50) {
                    tmp+=sprintf(" %s", obs[i]->GetShort());
                }
                else {
                    tmp+=" "+capitalize(obs[i]->GetKeyName())+ " the Long-Titled.";
                }
                if(obs[i]->GetSleeping() > 0) tmp+=" (%^YELLOW%^sleeping%^RESET%^) ";
                else if (obs[i]->GetProperty("afk")) tmp+=" (%^YELLOW%^afk%^RESET%^)";
                else if (query_idle(obs[i])>240 && obs[i]->GetInCombat()!=1)  tmp+=" (%^YELLOW%^idle%^RESET%^)";
                else if (in_edit(obs[i])) tmp+=" (%^RED%^edit%^RESET%^)";
                else if(obs[i]->GetInCombat())  tmp+=" (%^RED%^combat%^RESET%^)";
                tmp+="\n";
                p++;
            }
        }
        ret+=center(mud_name());
        ret+=SEP;
        ret+=tmp;
        ret+=SEP;
        x="There ";
        (p==1) ? x+="is " : x+="are ";
        x+=cardinal(p);
        (p==1) ? x+=" member " : x+=" members ";
        x+="of our reality.\n";
        ret+=center(x);
        if(check_string_length(ret)) this_player()->eventPrint(""+ret+"");
        else print_long_string(this_player(),ret);
        return 1;
    }
}

void help() {
    write("Syntax: <who [@mud]>\n\n"
            "Gives you a who list in abbreviated form from this mud or other muds on the I3 or IMC2 network.\n"
         );
}
