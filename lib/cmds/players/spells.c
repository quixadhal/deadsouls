/*    /cmds/player/spells.c
 *    From Dead Souls LPMud
 *    created by Descartes of Borg 961028
 *    Version: @(#) spells.c 1.3@(#)
 *    Last modified: 96/10/28
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object who = previous_object();
    string array spells = ({});
    string tmp;
    int len;
    
    if( creatorp(who) ) {
	who->eventPrint("Creators know no spells.");
	return 1;
    }
    tmp = "You know the following spells:\n";
    foreach(string spell, int val in who->GetSpellBook()) {
	spells += ({ sprintf("%:-20s: %:-3d%%", spell, val) });
    }
    if( !sizeof(spells) ) {
	tmp += "You know no spells.";
    }
    else {
	foreach(string spell in spells) {
	    int i = strlen(spell);
	    
	    if( i > len ) {
		len = i;
	    }
	}
	len = (who->GetScreen())[0]/(len+2);
	tmp += format_page(spells, len);
    }
    who->eventPrint(tmp);
    return 1;
}

string GetHelp() {
    return ("Syntax: <spells>\n\n"
	    "Lists all of your spells in your spell book with your "
	    "proficiency in each spell.\n\n"
	    "See also: skills, stats, status");
}
