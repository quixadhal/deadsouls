/* Hiccups@Frontiers, 12/16/96 */
/* Fixes by Haderach, 05 SEP 2005 */
/* RWHO check by Shadyman, 2006-Sep-12 */

#include <lib.h>
#include <privs.h>
#include <daemons.h>

inherit LIB_DAEMON;
mapping instinfo;
mixed instances;

#define SEP repeat_string("*=",39)+"*\n";

static int LevelSort(mixed one, mixed two){
    int i1, i2, alpha;
    string s1, s2;
    if(objectp(one)){
        i1 = one->GetLevel();
        s1 = one->GetName();
        if(creatorp(one)) alpha = 1;
    }
    if(objectp(two)){
        i2 = two->GetLevel();
        s2 = two->GetName();
        if(creatorp(two)) alpha = 1;
    }
    if(stringp(one) && sizeof(instinfo) && mapp(instinfo[one])){
        i1 = instinfo[one]["level"];
        s1 = one;
        if(instinfo[one]["creator"]) alpha = 1;
    }
    if(stringp(two) && sizeof(instinfo) && mapp(instinfo[two])){
        i2 = instinfo[two]["level"];
        s2 = two;
        if(instinfo[two]["creator"]) alpha = 1;
    }
    if(alpha) return strcmp(s1, s2);
    if(i1 > i2) return -1;
    else if(i2 > i1) return 1;
    else if(s1 && s2) return strcmp(s1, s2);
    return -1;
}

int cmd(string args) {
    int p;
    string x, tmp="", ret="";
    object *obs;

    if (args && args[0] == '@') { //If there's an @ in the first arg character, it's an rwho request.
        if( sizeof(args) <=1 ) return notify_fail("Get a remote who from where?"); //If there's only an @, tell them to RTFM.
        "/cmds/players/rwho"->cmd(args[1..]); //Pass it the mud name without the @
        return 1;
    } else {
        mixed arches, cres, players;
        p = 0;
        arches = sort_array(filter(users(), (: archp($1) :)),
                "LevelSort");
        cres = sort_array(filter(users(), (: !archp($1) && creatorp($1):)),
                "LevelSort");
        players = sort_array(filter(users(), (: !creatorp($1) :)),
                "LevelSort");
        obs = arches + cres + players;

        for (int i=0; i<sizeof(users()); i++) {
            string fnm;
            if(!obs[i] || !environment(obs[i])) continue;
            sscanf(last_string_element(base_name(obs[i]), "/"),"%s.%*s", fnm);
            if(fnm && fnm != obs[i]->GetKeyName()) continue;
            if(!obs[i]->GetInvis()){
                if(archp(obs[i])) tmp+="[%^BLUE%^ARCH%^RESET%^]";
                else if(creatorp(obs[i]) ) tmp+="[%^CYAN%^WIZ%^RESET%^]";
                else if(avatarp(obs[i]) ) tmp+="[%^GREEN%^AVATAR%^RESET%^]";
                else if(high_mortalp(obs[i]) ){
                    tmp+="[%^GREEN%^HIGH MORTAL%^RESET%^]";
                }
                else tmp+=sprintf("[%d]", obs[i]->GetLevel() );
                if(elderp(obs[i])) tmp+="[%^YELLOW%^ELDER%^RESET%^]";
                if(testp(obs[i]) && !archp(obs[i])){
                    tmp+="[%^B_CYAN%^%^BLACK%^TEST%^RESET%^]";
                }
                if(ambassadorp(obs[i])) tmp+="[%^YELLOW%^AMBASSADOR%^RESET%^]";
                tmp += ": ";
                if(sizeof(strip_colours(obs[i]->GetShort())) < 50) {
                    tmp+=sprintf(" %s", obs[i]->GetShort());
                }
                else {
                    tmp += " " + capitalize(obs[i]->GetKeyName())+ 
                        " the Long-Titled.";
                }
                if(obs[i]->GetSleeping() > 0){
                    tmp += " (%^BLUE%^sleeping%^RESET%^) ";
                }
                else if (obs[i]->GetProperty("afk")){
                    tmp += " (%^MAGENTA%^afk%^RESET%^)";
                }
                else if (query_idle(obs[i])>240 && obs[i]->GetInCombat()!=1){
                    tmp += " (%^YELLOW%^idle%^RESET%^)";
                }
                else if (in_edit(obs[i]) || obs[i]->GetCedmode()){
                    tmp += " (%^CYAN%^edit%^RESET%^)";
                }
                else if(obs[i]->GetInCombat()){
                    tmp += " (%^RED%^combat%^RESET%^)";
                }
                tmp += "%^RESET%^\n";
                p++;
            }
        }
        ret+=center(mud_name());
        ret+="\n";
        ret+=SEP;
        ret+=tmp;
        instances = singular_array(INSTANCES_D->GetInstances());
        if(ENABLE_INSTANCES) instances = ({ "global" });
        tmp = INSTANCES_D->GetMyInstanceName();
        if(tmp && instances) instances -= ({ tmp });
        tmp = "";

        foreach(string inst in sort_array(instances, 1)){
            mixed guys;
            mapping tmpmap;
            if(!inst) continue;
            arches = ({});
            cres = ({});
            players = ({});
            guys = ({});
            instinfo = INSTANCES_D->GetInstData()[inst];
            if(!sizeof(instinfo)){
                continue;
            }
            if(instinfo["mudname"]) inst = instinfo["mudname"];
            tmp += "\n"+center("--- Instance: "+capitalize(inst)+" ---\n");
            if(!mapp(instinfo["users"])){
                continue;
            }
            foreach(string name, mapping data in instinfo["users"]){
                string tmpname;
                if(name && mapp(data)){
                    tmpname = name;
                    if(data["arch"]) arches += ({ name });
                    else if(data["creator"]) cres += ({ name });
                    else players += ({ name });
                }
            }
            arches = sort_array(arches, "LevelSort");
            cres = sort_array(cres, "LevelSort");
            players = sort_array(players, "LevelSort");
            guys = arches + cres + players;
            foreach(string dude in guys){
                if(!dude) continue;
                tmpmap = instinfo["users"][dude];
                if(!sizeof(tmpmap)) continue;
                if(tmpmap["status"] < 1) continue;
                if(tmpmap["arch"])tmp += "[%^BLUE%^ARCH%^RESET%^]";
                else if(tmpmap["creator"]) tmp += "[%^CYAN%^WIZ%^RESET%^]";
                else tmp += "["+tmpmap["level"]+"]";
                tmp += ": " + tmpmap["title"];
                if(tmpmap["state"]) tmp += " " +tmpmap["state"];
                tmp += "%^RESET%^\n";
            }
            ret += tmp;
            tmp = "";
        }    
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

string GetHelp(){
    return ("Syntax: who [@mud]\n\n"
            "Gives you a who list in abbreviated form from this mud or other muds on the I3 or IMC2 network.");
}
