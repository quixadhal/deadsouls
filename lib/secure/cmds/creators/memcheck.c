/* /secure/cmd * from the Dead Souls LPC Library
 * gives information on leaked memory
 * NOTE: Only works with DEBUGMALLOC defined
 * created by Descartes of Borg 950206
 */

#include <lib.h>

inherit LIB_DAEMON;

int cmd(string unused) {
    check_memory();
    return 1;
}

void help() {
    message("help", "Syntax: <memcheck>\n\n"
      "Gives you information about leaked memory.  You should see "
      "nothing at all if things are going well.\n\n" 
      "See also: mstatus, netstat", 
      this_player());
}
