/*    /secure/cmds/adm/end.c
 *    from the Dead Souls LPC Library
 *    the command to reboot the MUD
 *    created by Descartes of Borg 950526
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    int x;

    if( !args || args == "" ) return "You must specify a delay in minutes.";
    if( !(x = to_int(args)) ) return "You must specify a delay in minutes.";
    if( x < 2 ) return "The delay must be at least 2 minutes.";
    EVENTS_D->eventReboot(x);
    message("system", "Reboot sequence initiated.", this_player());
    return 1;
}

void help() {
    message("help", "Syntax: <end [delay]>\n\n"
	    "Initiates a reboot sequence which will end in a reboot after "
	    "the specified delay period.  The delay must be specified in "
	    "minutes and it must be of at least two minutes.\n\n"
	    "See also: nextreboot, setreboot, shutdown", this_player());
}
