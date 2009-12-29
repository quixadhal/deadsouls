/*    /cmds/player/stats.c
 *    from the Dead Souls LPC Library 
 *    command to view stats
 *    created by Descartes of Borg 950409
 */

#include <lib.h>

inherit LIB_DAEMON;
object who;

mixed cmd(string args) {
    string *stats;
    string ret, tmp;
    int i, x, y;
    who = 0;

    if(args){
        if(!creatorp(this_player())){
            write("Try: help stats");
            return 1;
        }
        if(args == "me" || args == "myself") who = this_player();
        if(!who) who = find_player(lower_case(args));
        if(!who){
            object *creatures = get_livings(environment(this_player()));
            creatures = filter(creatures, 
                    (: answers_to(lower_case($(args)), $1) :));
            if(sizeof(creatures)) who = creatures[0];
        }
        if(!who){
            write("No such living thing found.");
            return 1;
        }
    }

    else who = this_player();

    ret = who->GetShort() + ", level " +
        who->GetLevel();
    if( (tmp = who->GetClass()) ) 
        ret += " " + capitalize(tmp);
    else ret += " Drifter";
    ret += " (" + who->GetRace() + ")\n";
    stats = map(sort_array(who->GetStats(), 1),
            (: sprintf("%:-20s: %:-3d", $1, 
                       who->GetStatLevel($1)) :));
    i = sizeof(stats);
    while(i--) if( (y = strlen(stats[i])) > x ) x = y;
    x = (this_player()->GetScreen())[0]/(x+2);
    ret += format_page2(stats, x);
    message("system", ret, this_player());
    return 1;
}

string GetHelp() {
    if(creatorp(this_player())){
        return ("Syntax: stats [player]\n\n"
                "Lists the stats of the specified player as well as "
                "the stat level. With no argument, it defaults to your "
                "own stats.\n"
                "See also: stat, status, score");
    }
    else {
        return ("Syntax: stats\n\n"
                "Lists all of your stats as well as how skilled you are "
                "at the stat in question.\n"
                "See also: stat, status, score");
    }
}
