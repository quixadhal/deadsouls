/*    /cmds/player/status.c
 *    from the Dead Souls LPC Library
 *    gives a player status information
 *    created by Descartes of Borg 950409
 */

#include <lib.h>

inherit LIB_DAEMON;

mixed cmd(string args) { return this_player()->eventDisplayStatus(); }

void help() {
    message("help", "Syntax: <status>\n\n"
            "Gives you information about your current physical status.\n\n"
            "See also: money, skills, stats", this_player());
}
