/*    /secure/cmds/creator/ed.c
 *    from the Dead Souls LPC Library
 *    creator command to enter the editor
 *    created by Descartes of Borg 950406
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    int x;

    if( args == "" || !args ) 
      args = user_path((string)this_player()->GetKeyName()) + "tmp.edit";
    if( (int)this_player()->GetForced() ) {
	message("system", "Someone tried to force you to edit "+ args +"\n" +
		identify(previous_object(-1)), this_player());
	return 0;
    }
    args = absolute_path( (string)this_player()->query_cwd(), args );
    if( (x = file_size(args)) == -2 ) 
      return "You cannot edit a directory!";
    else if( x == -1 )
      message("editor", args + ", new file, starting in input mode.\n",
        this_player());
    else message("editor", args + ", " + x + " bytes\n", this_player());
    this_player()->eventEdit(args, 1);
    return 1;
}

void help() {
    message("help", "Syntax: <ed ([filename])>\n\n"
	    "Enters the editor editing the file you name.\n\n"
	    "See also: led", this_player());
}
