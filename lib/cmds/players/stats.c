/*    /cmds/player/stats.c
 *    from the Dead Souls LPC Library 
 *    command to view stats
 *    created by Descartes of Borg 950409
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string *stats;
    string ret, tmp;
    int i, x, y;

    if( creatorp(this_player()) ) {
	message("system", "Creators have no stats, get a life.",
		this_player());
	return 1;
    }
    ret = "You are " +(string)this_player()->GetShort() + ", level " +
      (int)this_player()->GetLevel();
    if( (tmp = (string)this_player()->GetClass()) ) 
      ret += " " + capitalize(tmp);
    else ret += " Drifter";
    ret += " (" + (string)this_player()->GetRace() + ")\n";
    stats = map((string)this_player()->GetStats(),
		 (: sprintf("%:-20s: %:-3d", $1, 
			    (int)this_player()->GetStatLevel($1)) :));
    i = sizeof(stats);
    while(i--) if( (y = strlen(stats[i])) > x ) x = y;
    x = ((int *)this_player()->GetScreen())[0]/(x+2);
    ret += format_page(stats, x);
    message("system", ret, this_player());
    return 1;
}

void help() {
    message("help", "Syntax: <stats>\n\n"
	    "Lists all of your stats as well as how skilled you are "
	    "at the skill in question.\n\n"
	    "See also: stats, status", this_player());
}
