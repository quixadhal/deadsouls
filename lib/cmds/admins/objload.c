/*    /cmds/admins/objload.c
 *    From the Dead Souls V Object Library
 *    Provides information about loaded objects to find where trouble might be
 *    Created by Descartes of Borg 961204
 *    Version: @(#) objload.c 1.4@(#)
 *    Last modified: 96/12/04
 */

#include <lib.h>
#include <dirs.h>
#include <message_class.h>

inherit LIB_DAEMON;

static private int Realms = strlen(REALMS_DIRS);

mixed cmd(string args) {
    string array output = ({ "Creator Files:" });
    object array obs = objects((: file_name($1)[0..Realms-1] ==REALMS_DIRS :));
    mapping vals = ([]);

    foreach(object ob in obs) {
	string str = file_privs(file_name(ob));

	if( !vals[str] ) {
	    vals[str] = ({ 0, ({}) });
	}
	vals[str][0]++;
	vals[str][1] = ({ vals[str][1]..., ob });
    }
    foreach(string who, mixed array data in vals) {
	mapping mp = ([]);
	
	output += ({ who + ": " + data[0] });
	foreach(object ob in data[1]) {
	    mp[base_name(ob)]++;
	}
	foreach(string str, int count in mp) {
	    output += ({ "\t" + str + ": " + count });
	}
    }
    previous_object()->eventPage(output, MSG_SYSTEM);
    return 1;
}

string GetHelp(string unused) {
    return ("Syntax: <objload>\n\n"
	    "Provides you with information about how many objects "
	    "each creator has in the game.");
}
    
    
