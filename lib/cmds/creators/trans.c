/*    /cmds/creator/trans.c
 *    from the Foundation II LPC Library
 *    moves a player to your location
 *    created by Descartes of Borg 950425
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object ob;

    if( !args || args == "" ) return "Trans whom?";
    if( !(ob = find_player(convert_name(args))) && !(ob = find_living(args)) )
      return "No such being exists anywhere presently.";
    if( environment(ob) == environment(this_player()) ) 
      return (string)ob->GetCapName() + " is right here.";
    message("system", "You have been summoned by " + 
	    (string)this_player()->GetName() + ".", ob);
    if( !((int)ob->eventMoveLiving(environment(this_player()))) )
      return "Failed to move " + (string)ob->GetCapName() + ".";
    else message("system", "You trans " + (string)ob->GetCapName() + 
		 " to you.", this_player());
    return 1;
}

void help() {
    message("help", "Syntax: <trans [living]>\n\n"
	    "Brings a living thing to your location.\n\n"
	    "See also: goto", this_player());
}
