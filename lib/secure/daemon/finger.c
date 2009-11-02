/*    /secure/daemon/finger.c
 *    from the Foundation II LPC Library
 *    gives finger information about users
 *    created by Descartes of Borg 950402
 *    Version: @(#) finger.c 1.11@(#)
 *    Last modified: 96/12/14
 */

#include <lib.h>
#include <daemons.h>
#include <marriage.h>
#include <clock.h>
#include "finger.h"

inherit LIB_DAEMON;

private int LoginTime, Level, Btime, BirthTime, CreatorBirth, WhereBlock;
private string CurrentUser, Short, CapName, RealName, Email;
private string Town, Race, Gender, HostSite;
private string Class, Clan, Long;
private string Rank, WebPage, tmpfile;
private string array Titles, Religion;
private class marriage array Marriages;

static void create() {
    daemon::create();
    SetNoClean(1);
}

static int LoadPlayer(string path){
    return unguarded( (: RestoreObject($(path)) :) );
}

varargs string GetFinger(string who, int html) {
    mapping mail_stat;
    object ob;
    string ret;
    mixed creator;

    tmpfile = 0;

    if( !who ) {
        object array people;
        string array lines = ({});
        object person;
        int i;
        int array screen;

        screen = this_player()->GetScreen();
        if( screen && sizeof(screen) ) i = screen[0]; else i = 80;
        ret = center(mud_name(), i) + "\n\n";
        people = filter(users(), (: !($1->GetInvis()) :));
        foreach(person in people) {
            string str, clas, town;
            str = person->GetShort();
            if( !str ) str = person->GetName();
            if( !str ) continue;
            if( creatorp(person) ) clas = "creator";
            else clas = person->GetClass();
            if( clas ) clas = capitalize(clas);
            else clas = "Drifter";
            town = person->GetTown();
            if( !town ) town = "Homeless";
            str = sprintf("%:-40s %:-11s %s", str, clas, town);
            lines += ({ str });
        }
        ret += implode(lines, "\n");
        return ret;
    }
    creator = 0;
    if( !user_exists(who) || (!LoadPlayer(DIR_PLAYERS "/" +
                    who[0..0] + "/" + who) && !(creator = LoadPlayer(DIR_CRES "/" +
                            who[0..0] + "/" + who))) ){
        return capitalize(who) + " is unknown to " + mud_name() + ".";
    }
    if( !strsrch(player_save_file(who), DIR_CRES) ) creator = 1;
    ret = "%^STRONG%^" + replace_string(GetTitle(), "$N", CapName) +
        "%^/STRONG%^BR%^\n";
    if( Long ) {
        ret += replace_string(Long, "$N", CapName) + "%^BR%^\n";
    }
    if( WebPage ) {
        ret += "Home Page: %^HREF%^" + WebPage + "%^>%^";
        if( html ) {
            ret += WebPage + "%^/A%^BR%^\n";
        }
        else {
            ret += "%^/A%^BR%^\n";
        }
    }
    else {
        ret += "No home page.%^BR%^\n";
    }
    if( creator ) {
        ret += CapName + " is a " + Gender + " Creator of %^I%^" + mud_name() +
            "%^/I%^.%^BR%^\n";
        ret += "Immortality Date: " + ctime(CreatorBirth) + "%^BR%^\n";
    }
    else {
        string yrstr;
        int yr;

        ret += CapName+" the "+Gender+" "+Race+" "+(Class || "drifter");
        if( Clan ) ret += " of the " + pluralize(Clan);
        if( creatorp(this_player()) && Level ) ret += " ("+Level+")";
        ret += " is a " + Rank+" of " + Town + ".%^BR%^\n";
        Btime = BirthTime - (18 * YEAR);
        if( (yr = query_year(Btime)) < 0 ){
            yrstr = -yr + " BN";
        }
        else {
            yrstr = "" + yr;
        }
        ret += sprintf("Birthday: the %d%s day of %s, %s",
                query_date(Btime), ordinal(query_date(Btime)),
                query_month(Btime), yrstr) + "%^BR%^\n";
    }
    if(!this_player() || !archp(this_player())) Email = "Unlisted";
    if( !Email ) Email = "#CHANGE";
    ret += "%^TABLE%^TR%^TD%^" +
        sprintf("In real life: %:-30s %%^/TD%%^TD%%^Email: %s", RealName ,
                (Email[0]!='#' || (this_player(1) && archp(this_player(1))))
                ? (Email[0] == '#' ? Email[1..] : Email) : "Unlisted") +
        "%^/TD%^/TR%^\n";
    ret += "%^TR%^TD%^" +
        sprintf("Religion: %:-34s %%^/TD%%^TD%%^Spouse: %s",
                (!creator && Religion[1]) ? Religion[1] : "Agnostic",
                !sizeof(Marriages) ? "Single" :
                (((class marriage)Marriages[0])->DivorceDate ?
                 "Divorced" : ((class marriage)Marriages[0])->Spouse)) +
        "%^/TD%^/TR%^/TABLE%^\n";
    if( (ob = find_player(who)) && (!ob->GetInvis()) ) {
        if( !interactive(ob) ) ret += "Currently net-dead";
        else ret += "On since " + ctime(LoginTime);
    }
    else ret += "Last on " + ctime(LoginTime);
    if( this_player(1) && creatorp(this_player(1)) && (!AUTO_WIZ ||
                master()->valid_apply(({ "SECURE", "ASSIST" }))) ){
        ret += " from " + HostSite + "%^BR%^\n";
    }
    else ret += "%^BR%^\n";
    mail_stat = FOLDERS_D->mail_status(who);
    if( mail_stat["unread"] )
        ret += CapName + " has " + consolidate(mail_stat["unread"],
                "an unread letter") + ".%^BR%^\n";
    if( unguarded( (: file_size, user_path(who) + ".project" :) ) > 0 )
        ret += "Project: %^PRE%^" +
            unguarded( (: read_file, user_path(who) + ".project" :) ) +
            "%^/PRE%^\n";
    if( unguarded( (: file_size, user_path(who) + ".plan" :) ) > 0 )
        ret += "Plan:%^PRE%^\n" +
            unguarded( (: read_file, user_path(who) + ".plan" :) ) + "%^/PRE%^\n";
    return ret;
}

mixed array GetRemoteFinger(string who) {
    mixed array tmp;
    string plan;
    int creator;
    object ob;

    who = convert_name(who);
    creator = 0;
    if( !user_exists(who) ) return 0;
    else if( !RestoreObject(DIR_PLAYERS "/" + who[0..0] + "/" + who) &&
            !(creator = RestoreObject(DIR_CRES "/" +
                    who[0..0] + "/" + who)) ) return 0;
    else if( !strsrch(player_save_file(who), DIR_CRES) ) creator = 1;
    ob = find_player(who);
    if( unguarded( (: file_size, user_path(who) + ".plan" :) ) > 0 ) {
        plan = unguarded( (: read_file, user_path(who) + ".plan", 1, 5 :) );
        if( strlen(plan) ) {
            plan = "Plan: (truncated to 5 lines)\n" + plan;
            if( plan[<1] != '\n' ) plan += "\n";
        }
        else plan = 0;
    }
    if(!this_player() || !archp(this_player()))  Email = "Unlisted";
    if( !Email ) Email = "#CHANGE";
    tmp = ({ CapName, GetTitle(), RealName, (Email[0] != '#' ? Email : 0),
            ctime(LoginTime), ((ob && interactive(ob) && !(ob->GetInvis())) ? query_idle(ob) : -1),
            0, (creator ? "Creator" : "" + Level), plan });
    return tmp;
}

string GetTitle() {
    if( Short ) return Short;
    else if( Titles && sizeof(Titles) ) return Titles[0];
    else return CapName;
}
