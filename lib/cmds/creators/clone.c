/*    /cmds/creator/clone.c
 *    from the Dead Souls LPC Library
 *    clones objects for cres
 *    created by Descartes of Borg 950409
 *    Version: @(#) clone.c 1.2@(#)
 *    Last modified: 96/10/08
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    object ob;
    string file, res, nom;

    if( args == "" || !args ) return "Clone what?";
    file = absolute_path((string)this_player()->query_cwd(), args);
    if( file[<2..] != ".c" ) file = file + ".c";
    if( file_size(file) < 0 ) return "No such file " + file;
    if( res = catch(ob = new(file)) ) 
      return "Error in cloning object: " + res;
    if( !ob ) return "Failed to clone file: " + file;
    if( !((int)ob->eventMove(this_player())) &&
       !((int)ob->eventMove(environment(this_player()))) ) {
	message("system", "Failed to properly move the object.",
		this_player());
	return 1;
    }
    if( !(nom = (string)ob->GetShort()) ) nom = "something peculiar";
    if( !(res = (string)this_player()->GetMessage("clone", ob)) )
      res = (string)this_player()->GetName() + " clones " + nom + ".";
    message("info", "You clone " + nom + " (" + file + ").",
	    this_player());
    message("other_action", res, environment(this_player()), 
	    ({ this_player() }));
    return 1;
}

string GetHelp() {
    return ("Syntax: <clone [filename]>\n\n"
	    "Clones the object from the code stored in the file named.  "
	    "If for some reason the object cannot be moved to your "
	    "inventory (it is alive, you cannot carry it, etc.), then "
	    "it will be moved into your environment.\n\n"
	    "See also: dest, message");
}
