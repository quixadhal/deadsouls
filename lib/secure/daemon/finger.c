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
 
private int LoginTime, Level, BirthTime, CreatorBirth, WhereBlock;
private string CurrentUser, Short, CapName, RealName, Email;
private string Town, Race, Gender, HostSite;
private string Class, Guild, Long;
private string Rank, WebPage;
private string array Titles, Religion;
private class marriage array Marriages;

static void create() {
    daemon::create();
    SetNoClean(1);
}
 
varargs string GetFinger(string who, int html) {
    mapping mail_stat;
    object ob;
    string ret;
    mixed creator;
 
    if( !who ) {
        object array people;
        string array lines = ({});
        object person;
        int i, maxi;
        int array screen;
 
        screen = (int array)this_player()->GetScreen();
        if( screen && sizeof(screen) ) i = screen[0]; else i = 80;
        ret = center(mud_name(), i) + "\n\n";
        people = filter(users(), (: !((int)$1->GetInvis()) :));
        foreach(person in people) {
            string str, clas, town;
 
            str = (string)person->GetShort();
            if( !str ) str = (string)person->GetName();
            if( !str ) continue;
            if( creatorp(person) ) clas = "creator";
            else clas = (string)person->GetClass();
            if( clas ) clas = capitalize(clas);
            else clas = "Drifter";
            town = (string)person->GetTown();
            if( !town ) town = "Homeless";
            str = sprintf("%:-40s %:-11s %s", str, clas, town);
            lines += ({ str });
        }
        ret += implode(lines, "\n");
        return ret;
    }
    if( who != CurrentUser ) {
	creator = 0;
        if( !user_exists(who) )
          return capitalize(who) + " has never visited " + mud_name() + ".";
        else if( !unguarded( (: restore_object, DIR_PLAYERS "/" + who[0..0] +
                              "/" + who :)) &&
                  !(creator = unguarded( (: restore_object, DIR_CRES "/" +
                              who[0..0] + "/" + who :) ) )) return 0;
        CurrentUser = who;
    }
    else if( !strsrch(save_file(who), DIR_CRES) ) creator = 1;
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
	if( Guild ) ret += " of the " + pluralize(Guild);
        if( creatorp(this_player()) && Level ) ret += " ("+Level+")";
	ret += " is a " + Rank+" of " + Town + ".%^BR%^\n";
	BirthTime = BirthTime - (18 * YEAR);
	if( (yr = query_year(BirthTime)) < 0 ) yrstr = -yr + " BN";
	else yrstr = "" + yr;
	ret += sprintf("Birthday: the %d%s day of %s, %s",
		       query_date(BirthTime), ordinal(query_date(BirthTime)),
		       query_month(BirthTime), yrstr) + "%^BR%^\n";
    }
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
    if( !WhereBlock || (this_player(1) && archp(this_player(1))) )
      ret += " from " + HostSite + "%^BR%^\n";
    else ret += "%^BR%^\n";
    mail_stat = (mapping)FOLDERS_D->mail_status(who);
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
    if( who != CurrentUser ) {
	creator = 0;
        if( !user_exists(who) ) return 0;
        else if( !unguarded( (: restore_object, DIR_PLAYERS "/" + who[0..0] +
                              "/" + who :)) &&
                !(creator = unguarded( (: restore_object, DIR_CRES "/" +
					who[0..0] + "/" + who :) ) )) return 0;
        CurrentUser = who;
    }
    else if( !strsrch(save_file(who), DIR_CRES) ) creator = 1;
    ob = find_player(who);
    if( unguarded( (: file_size, user_path(who) + ".plan" :) ) > 0 ) {
        plan = unguarded( (: read_file, user_path(who) + ".plan", 1, 5 :) );
	if( strlen(plan) ) {
	    plan = "Plan: (truncated to 5 lines)\n" + plan;
	    if( plan[<1] != '\n' ) plan += "\n";
	}
	else plan = 0;
    }
    if( !Email ) Email = "#CHANGE";
    tmp = ({ CapName, GetTitle(), RealName, (Email[0] != '#' ? Email : 0),
	     ctime(LoginTime), (ob && interactive(ob) ? query_idle(ob) : -1),
	     0, (creator ? "Creator" : "" + Level), plan });
    return tmp;
}
 
string GetTitle() {
    if( Short ) return Short;
    else if( Titles && sizeof(Titles) ) return Titles[0];
    else return CapName;
}
