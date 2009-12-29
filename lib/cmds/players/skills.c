/*    /cmds/player/skills.c
 *    from the Dead Souls LPC Library
 *    command to view skills
 *    created by Descartes of Borg 950409
 *    - Moved map function to GetLine for readability, added a max skill
 *      level display for each skill, grouped in classes. (Blitz 960122/0404)
 */

#include <lib.h>

inherit LIB_DAEMON;

string GetLine(string skill) {
    int x, max;
    mapping mp = this_player()->GetSkill(skill);

    if( !sizeof(mp) ) return 0;
    x = to_int(percent(mp["points"],
                this_player()->GetMaxSkillPoints(skill, mp["level"])));
    max = this_player()->GetMaxSkillLevel(skill);
    if( max < mp["level"] ) max = mp["level"];
    return sprintf("%:-20s: %:-6s (%d%%)", skill,
            (mp["level"] + "/" + max), x);
}

mixed cmd(string args) {
    string *skills, *primes, *secs;
    string ret, tmp;
    int x, scr;

    ret = "You are " +this_player()->GetShort() + ", level " +
        this_player()->GetLevel();
    if( (tmp = this_player()->GetClass()) )
        ret += " " + capitalize(tmp);
    else ret += " Drifter";
    ret += " (" + this_player()->GetRace() + ")\n";
    scr = (this_player()->GetScreen())[0];

    skills = sort_array(this_player()->GetSkills(), 1);
    if( !sizeof(skills) ) {
        ret += "You are without skills.\n";
        this_player()->eventPrint(ret);
        return 1;
    }
    skills = skills - (primes = filter(skills,
                (: this_player()->GetSkillClass($1) == 1 :)));
    skills = skills - (secs = filter(skills,
                (: this_player()->GetSkillClass($1) == 2 :)));
    skills = map(skills, (: GetLine :));
    primes = map(primes, (: GetLine :));
    secs   = map(secs, (: GetLine :));
    foreach(mixed sarray in ({ primes, secs, skills }) ) {
        int y, i = sizeof(sarray);
        while(i--) if( (y = strlen(sarray[i])) > x ) x = y;
    }
    x = scr/(x+2);
    ret += "%^BOLD%^%^BLUE%^Primary skills:%^RESET%^\n";
    ret += format_page2(primes, x);
    ret += "\n%^BOLD%^%^BLUE%^Secondary skills:%^RESET%^\n";
    ret += format_page2(secs, x);
    ret += "\n%^BOLD%^%^BLUE%^Other skills:%^RESET%^\n";
    ret += format_page2(skills, x);
    this_player()->eventPage(explode(ret, "\n"));
    return 1;
}

string GetHelp(){
    return "Syntax: skills\n\n"
        "Lists all of your skills as well as how skilled you are "
        "at the skill in question.\n"
        "See also: stats, status";
}
