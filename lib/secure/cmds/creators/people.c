/*    /cmd *    from Dead Souls
 *    a listing of users for creators and ambassadors
 *    created by Descartes of Borg 940724
 */

#include <lib.h>
#include <localtime.h>
#include <dirs.h>
#include <daemons.h>

inherit LIB_DAEMON;

static private int *__SortFlags;

static private string query_people_time();
private string calculateFormatString(int screenSize);
object *whom, *who, *display;
string *args;

string GetBaseName(object ob){
    string ret;
    if(ob) ret = capitalize(cleaned_end(base_name(ob)));
    if(!ret && ob) ret = ob->GetCapName();
    if(!ret) ret = "";
    return ret;
}

mixed room_env(object ob){
    string *riders;
    object env = environment(ob);
    if(!env) return 0;
    if(!living(env)) return env;
    if(arrayp(riders = env->GetRiders()) && member_array(ob, riders) != -1 &&
            environment(env)) env = environment(env);
    return env;
}

int cmd(string str) {
    string msg, tmp1, tmp2;
    int i, maxi, aflag, bflag, cflag, eflag, gflag, hflag, lflag, mflag;
    int nflag, pflag, rflag, sflag, uflag;
    int screenSize;
    string formatString;
    string bar;

    whom = ({});

    if(!str || str == "" || strlen(str) == 1 || str[0] != '-') args = ({});
    else args = explode(str[1..strlen(str)-1], "");
    i = sizeof(args);
    while(i--) {
        switch(args[i]) {
            case "a": aflag = 1; break; /* list arches */
            case "b": bflag = 1; break; /* list ambassadors */
            case "c": cflag = 1; break; /* list creators */
            case "e": eflag = 1; break; /* sort by race */
            case "g": gflag = 1; break; /* sort by age */
            case "h": hflag = 1; break; /* list high mortals */
            case "l": lflag = 1; break; /* sort by level */
            case "m": mflag = 1; break; /* page through eventPage */
            case "n": nflag = 1; break; /* list newbies */
            case "p": pflag = 1; break; /* list regular mortals */
            case "r": rflag = 1; break; /* sort by realm location */
            case "s": sflag = 1; break; /* sort by class */
            case "u": uflag = 1; break; /* list undead */
        }
    }
    who = filter(users(), "filter_invis", this_object());
    foreach(object real_person in who){
        if(sizeof(base_name(real_person))) whom += ({ real_person });
    }

    who = whom;

    if(!aflag && !bflag && !cflag && !hflag && !nflag && !pflag && !uflag)
        display = who;
    else {
        display = ({});
        if(aflag) display += filter(who, "filter_arches", this_object());
        if(bflag) display += filter(who, "filter_ambass", this_object());
        if(cflag) display += filter(who, "filter_cres", this_object());
        if(hflag) display += filter(who, "filter_hms", this_object());
        if(nflag) display += filter(who, "filter_newbie", this_object());
        if(pflag) display += filter(who, "filter_mortal", this_object());
        if(uflag) display += filter(who, "filter_undead", this_object());
        display = distinct_array(display);
    }
    if(!eflag && !gflag && !lflag && !rflag && !sflag) 
        maxi = sizeof(display=sort_array(display,"general_sort",this_object()));
    else {
        __SortFlags = ({ eflag, gflag, lflag, rflag, sflag });
        maxi = sizeof(display = sort_array(display,"special_sort",this_object()));
    }
    screenSize = ((int*)this_player()->GetScreen())[0];
    formatString = calculateFormatString(screenSize);
    bar = "--------------------------------------------------------------------------";
    for(i = 75; i < screenSize; i++) bar += "-";
    bar += "\n";
    msg = bar;
    tmp1 = " " + maxi + ((maxi != 1) ? " people" : " person") + " in current sort ";
    tmp2 = query_people_time() + " ";
    i = sizeof(tmp1) + sizeof(tmp2);
    msg += tmp1;
    for(i = sizeof(tmp1) + sizeof(tmp2) + 1; i < screenSize; i++) msg += " ";
    msg += tmp2 + "\n";
    msg += bar;
    msg += implode(map_array(display, "map_info", this_object(), formatString), "\n") + "\n";
    msg += bar;
    msg += center(mud_name(), screenSize - 1);
    if(!check_string_length()) mflag = 1;
    if(!mflag && check_string_length(msg)) this_player()->eventPrint(msg);
    else print_long_string(this_player(),msg);
    return 1;
}

static int filter_invis(object ob) {
    if(!ob || !sizeof(base_name(ob))) return 0;
    if(!(ob->GetKeyName())) return 0;
    if(!(ob->GetInvis(this_player()))) return 1;
    if(archp(this_player())) return 1;
    if(archp(ob)) return 0;
    return 1;
}

static int filter_arches(object ob) { return archp(ob); }

static int filter_ambass(object ob) { return ambassadorp(ob); }

static int filter_cres(object ob) { return (creatorp(ob) && !archp(ob)); }

static int filter_hms(object ob) { return high_mortalp(ob); }

static int filter_newbie(object ob) {
    return (!creatorp(ob) && !ambassadorp(ob) && (MAX_NEWBIE_LEVEL >=
                ob->GetLevel()));
}

static int filter_mortal(object ob) {
    if(creatorp(ob) || high_mortalp(ob) || ambassadorp(ob)) return 0;
    if(ob->GetLevel() <= MAX_NEWBIE_LEVEL) return 0;
    return 1;
}

static int filter_undead(object ob) { return ob->query_ghost(); }

static int general_sort(object alpha, object beta) {
    int x, y;

    if(archp(alpha)) {
        if(!archp(beta)) return -1;
        else return strcmp(GetBaseName(alpha), 
                GetBaseName(beta));
    }
    else if(archp(beta)) return 1;
    if(creatorp(alpha)) {
        if(!creatorp(beta)) return -1;
        else return strcmp(GetBaseName(alpha),
                GetBaseName(beta));
    }
    else if(creatorp(beta)) return 1;
    if(ambassadorp(alpha)) {
        if(!ambassadorp(beta)) return -1;
        else return strcmp(GetBaseName(alpha),
                GetBaseName(beta));
    }
    else if(ambassadorp(beta)) return 1;
    if((x = alpha->GetLevel()) > (y = beta->GetLevel()))
        return -1;
    else if(x < y) return 1;
    else return strcmp(GetBaseName(alpha),
            GetBaseName(beta));
}

static int special_sort(object alpha, object beta) {
    string a, b;
    int x, y;

    if(__SortFlags[4]) {
        if((a=alpha->query_class())!=(b=beta->query_class())) {
            if(!a) a = "zzzz";
            if(!b) b= "zzzz";
            return strcmp(a, b);
        }
    }
    if(__SortFlags[0]) {
        if((a=alpha->query_race()) != (b=beta->query_race())) {
            if(!a) a = "zzzz";
            if(!b) b = "zzzz";
            return strcmp(a, b);
        }
    }
    if(__SortFlags[3]) {
        if((a = file_name(room_env(alpha))) != 
                (b = file_name(room_env(beta)))) return strcmp(a, b);
    }
    if(__SortFlags[2]) {
        if((x = alpha->GetLevel()) != (y=beta->GetLevel())) {
            if(x > y) return -1;
            else return 1;
        }
    }
    if(__SortFlags[1]) {
        if((x = alpha->GetAge()) != (y = beta->GetAge())) {
            if(x > y) return -1;
            else return 1;
        }
    }
    return 0;
}

private string calculateFormatString(int screenSize) {
    int nomSize = (((screenSize - 21) * 12) / 54);
    int ipSize = (((screenSize - 21) * 18) / 54);
    int envSize = (((screenSize - 21) * 24) / 54);

    if(nomSize < 12) nomSize = 12;
    if(ipSize < 18) ipSize = 18;
    if(envSize < 24) envSize = 24;

    return "%:-5s %:-2s %:-" + nomSize + "s %:-" + ipSize
        + "s %:-5s %:-3s %:-" + envSize + "s";
}

static string map_info(object ob, string formatString) {
    string age, nom, blk, lev, ip, env, idle;
    int x;

    x = ob->GetAge();
    if(x > 86400) age = sprintf("%:-2d D", x/86400);
    else if(x > 7200) age = sprintf("%:-2d h", x/3600);
    else age = sprintf("%:-2d m", x/60);
    nom = GetBaseName(ob);
    if(ob->GetInvis()) nom = "("+nom+")";
#ifndef __DSLIB__
    if(in_edit(ob) || in_input(ob)) nom = "["+nom+"]";
#else
    if(ob->GetCharmode()){
        if(in_edit(ob)) nom = "["+nom+"]";
    }
    else if(in_edit(ob) || in_input(ob)) nom = "["+nom+"]";
#endif
    if(creatorp(ob)) {
        if(ob->GetBlocked("all")) blk = "ACG";
        else {
            if(ob->GetBlocked("cre")) blk = " C";
            else blk = "  ";
            if(ob->GetBlocked("gossip")) blk += "G";
            else blk += " ";
        }
    }
    else blk = "   ";
    if(!(x = ob->GetLevel())) lev = "-";
    else lev = x+"";
    if((x = query_idle(ob)) < 60) idle = "";
    else if(x >= 3600) idle = sprintf("%:-3d h", x/3600);
    else idle = sprintf("%:-2d m", x/60);
    ip = query_ip_name(ob);
    if(ip == "0.0.0.0" || ip == "w.x.y.z") ip = "";
    if(!room_env(ob)) env = "no environment";
    else env = file_name(room_env(ob));
    if(!strsrch(env, REALMS_DIRS)) 
        env = "~" + env[strlen(REALMS_DIRS)+1..];
    else if(!strsrch(env, DOMAINS_DIRS))
        env = "^"+env[strlen(DOMAINS_DIRS)+1..strlen(env)-1];
    return sprintf(formatString, age, lev, nom, ip, idle, blk, env);
}

static private string query_people_time() {
    string tzone;
    if(this_player()) tzone = this_player()->GetProperty("timezone");
    if(!tzone || !valid_timezone(tzone)) tzone = query_tz();
    return tzone + " time is: " + local_ctime(time(), tzone);
}

string GetHelp() {
    return ("Syntax: people [-abceghlmnprsu]\n\n"
            "Gives you a listing of people on "+mud_name()+".  Output is "
            "formatted into columns with the following meanings:\n"
            "    amount of time spent total on "+mud_name()+"\n"
            "    level\n"
            "    name\n"
            "    address from which the person is connecting\n"
            "    amount of time idle\n"
            "    blocking information, if any... A is all, C cre, G gossip\n"
            "    current environment file name\n\n"
            "Command flags have the following meanings:\n"
            "    -a list all arches\n"
            "    -b list all ambassadors\n"
            "    -c list all non-arch creators\n"
            "    -e sort the listing by race\n"
            "    -g sort the listing by age\n"
            "    -h list all high mortals\n"
            "    -l sort the listing by level\n"
            "    -m pipe the listing through the more pager\n"
            "    -n list all newbies\n"
            "    -p list all non-newbie and non-high mortal mortals\n"
            "    -r sort the listing by current environment\n"
            "    -s sort the listing by class\n"
            "    -u list all undead people\n\n"
            "People in the editor or in mail or posting will be "
            "marked with [].  "
            "People who are invisible to others, but not to you will be marked "
            "with ().  The default sort is by arch, then by creator, then by "
            "ambassador, then by level.  You can perform multiple sorts, but "
            "you cannot change the priority.  Sorts priority is: class, race, "
            "realm, level, age.  You can also include multiple groups in your "
            "listing. Fart.\nSee also: who, users");
}
