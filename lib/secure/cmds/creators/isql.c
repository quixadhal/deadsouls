/*    /secure/cmds/creators/isql.c
 *    from the Dead Souls V Object Library
 *    Interactive SQL interface for muds supporting LPC SQL
 *    created by Descartes of Borg 960301
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    if( !args || args == "" ) return "Connect to which database?";
    else return previous_object()->eventDatabaseConnect(args);
}

string GetHelp(string str) {
    return ("Syntax: <isql DATABASE>\n\n"
	    "Allows you to interactively issue SQL statements.  Besides "
	    "SQL statements, you may also issue the follow commands on "
	    "lines by themselves:\n"
	    "\tgo\n"
	    "\tSends the current buffer to the database engine for "
	    "interpretation.\n"
	    "\tquit\n"
	    "\tExits the ISQL interface.");
}
