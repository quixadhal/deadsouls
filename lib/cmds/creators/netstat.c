/*    /cmds/creator/netstat.c
 *    from the Dead Souls LPC Library
 *    gives socket status information
 *    created by Descartes of Borg 921123
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) {
    string *arr;

    arr = filter(explode(dump_socket_status(), "\n"), (: $1[0..1] != "-1" :));
    this_player()->eventPage(arr, "system");
    return 1;
}

void help() {
    message("help", "Syntax: <netstat>\n\n"
	    "Gives you information about sockets being used by the LPC "
	    "server through the MudOS socket efuns.\n\n"
	    "See also: callouts, dumpallobj, mstatus", this_player());
}
