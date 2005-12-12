/*    /cmds/creators/wiz.c
 *    Created by Zin@Frontiers
 *    Sun Sep 21 21:00:16 1997 EDT
 */

#include <lib.h>
#include <daemons.h>

inherit LIB_DAEMON;

mixed cmd(string str) {
    write("You speed to the Creators' Hall.\n");
    this_player()->eventMoveLiving("/domains/default/room/wiz_hall");
    return 1;
}

void help() {
    message("help", "Syntax: <center>\n\n"
      "This command will move you to the Creators' Hall.\n\n",
      this_player());
}                                                    
