/*    /cmds/player/converse.c
 *    from the Dead Souls LPC Library
 *    allows a person to talk without having to use the say command
 *    created by Descartes of Borg 950419
 */

#include <lib.h>
#include <talk_type.h>
#include <commands.h>

inherit LIB_DAEMON;

static void eventChat(string args);

mixed cmd(string args) {
    message("system", "Entering conversation mode.  Continue entering things "
	    "you wish 'said' until done.", this_player());
    message("system", "When done, enter a '.' alone on a line like in edit.",
	    this_player());
    message("system", "______________________________________________________"
            , this_player());
    input_to( (: eventChat :));
    return 1;
}

static void eventChat(string args) {
    if( trim(args) == "." ) {
	message("system", "Exiting conversation mode.", this_player());
	return;
    }
    else this_player()->eventSpeak(0, TALK_LOCAL, args);
    input_to( (: eventChat :) );
}

void help() {
    message("help", "Syntax: <converse>\n\n"
	    "Puts you into conversation mode, which means that anything you "
	    "type will act as if you are placing the \"say\" command before "
	    "it.  Once in conversation mode, you exit the same way you do "
	    "from the mail or bulletin board editor, that is, by typing "
	    "a '.' alone on a line.  You may also issue commands while "
	    "in conversation mode by putting a ! before whatever you type.\n\n"
	    "See also: say, tell", this_player());
}
