/*    /cmds/player/screen.c
 *    from the Foundation II LPC Library
 *    sets width and height for player terminals
 *    created by Descartes of Borg 950328
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    int *screen;
    int h, w;

    if( args == "" || !args ) 
      return "You need to specify both width and height.";
    if( sscanf(args, "%d %d", w, h) != 2 )
      return "You need to specify both width and height.";
    this_player()->SetScreen(w, h);
    message("system", "Screen set to " + w + " by " + h + ".", this_player());
    return 1;
}

void help() {
    message("help", "Syntax: <screen [width] [height]>\n\n"
	    "Sets the dimensions of your computer screen so that " +
	    mud_name() + " knows how to send information to your screen.\n\n"
	    "See also: brief, terminal", this_player());
}
