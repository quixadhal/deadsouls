/*    /cmds/players/customize.c
 *    from the Dead Souls V Object Library
 *    customize
 *    customize STAT AMOUNT
 *    created by Descartes of Borg 950113
 */

#include <lib.h>
#include <message_class.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    mixed tmp;
    string stat;
    int amt;
    
    if( !args || args == "" ) {
	amt = (int)this_player()->GetCustomStats();
	this_player()->eventPrint("You have " + amt + " points left to "
				  "spend on stats.", MSG_SYSTEM);
	return 1;
    }
    amt = to_int((tmp = explode(args, " "))[<1]);
    if( sizeof(tmp) == 1 ) return "Customize which stat how much?";
    stat = implode(tmp[0..<2], " ");
    tmp = (mixed)this_player()->eventCustomizeStat(stat, amt);
    if( stringp(tmp) ) return tmp;
    if( !tmp ) return "Failed to raise stat.";
    this_player()->eventPrint("Your " + stat + " is now at " + tmp +
			      ", and you have " +
			      (int)this_player()->GetCustomStats() +
			      " points left to spend.", MSG_SYSTEM);
    return 1;
}

string GetHelp(string str) {
    return ("Syntax: <customize>\n"
	    "        <customize STAT AMOUNT>\n\n"
	    "Allows you to spend customization points to boost the "
	    "stats with which you were born.  Once you are down to 0 "
	    "customization points, you never get any back.\n"
	    "Using this command without arguments tells you how many "
	    "points you have left to spend.");
}
