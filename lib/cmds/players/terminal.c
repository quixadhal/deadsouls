/*    /cmds/player/terminal.c
 *    from the Foundation II LPC Library
 *    allows a person to set their terminal manually
 *    created by Descartes of Borg 950501
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    if( !args || args == "" ) return "Set it to what?";
    message("system", "Terminal set to " + 
	    (string)this_player()->SetTerminal(args) + ".", this_player());
    return 1;
}

void help() {
    message("help", "Syntax: <terminal [term type]>\n\n"
	    "Allows you to set your terminal type manually in the " 
	    "event the MUD does not automatically recognize the proper "
	    "setting.\n\n"
	    "See also: screen", this_player());
}
