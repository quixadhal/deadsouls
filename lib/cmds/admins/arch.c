/*    /cmds/creators/wiz.c
 *    Created by Zin@Frontiers
 *    Sun Sep 21 21:00:16 1997 EDT
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    write("You speed to the Arch room.\n");
    this_player()->eventMoveLiving("/secure/room/arch");
    return 1;
}

void help() {
    message("help", "Syntax: <arch>\n\n"
      "This command will move you to the Arch room.\n\n",
      this_player());
}                                                    
